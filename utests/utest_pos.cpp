#include "../src/volume.h"

int main()
{
	volume testVol;

	const float origin[3] = {-12.3123, 12.323, 1.323};
	const float res[3] = {0.123, 0.2323, 0.23};
	testVol.set_origin(origin[0], origin[1], origin[2]);
	testVol.set_res(res[0], res[1], res[2]);

	for (uint8_t iDim = 0; iDim < 3; iDim++)
	{
		if (testVol.get_pos(0, iDim) != origin[iDim])
		{
			printf("All vectors should start at their origin\n");
			throw "Invalid value";
		}
	}

	if (testVol.get_pos0(1) != testVol.get_pos(1, 0))
	{
		printf("get_pos0(1) and get_pos(1, 0) should be identical\n");
		throw "Invalid value";
	}

	if (testVol.get_pos1(1) != testVol.get_pos(1, 1))
	{
		printf("get_pos1(1) and get_pos(1, 1) should be identical\n");
		throw "Invalid value";
	}

	if (testVol.get_pos2(1) != testVol.get_pos(1, 2))
	{
		printf("get_pos2(1) and get_pos(1, 2) should be identical\n");
		throw "Invalid value";
	}

	testVol.set_dim(1, 1, 1);

	// length of each dimension should not we one voxel, meaning the same as res
	for (uint8_t iDim = 0; iDim < 3; iDim++)
	{
		if (testVol.get_length(iDim) != testVol.get_res(iDim))
		{
			printf("Length should be the same as res if dim is 1\n");
			throw "InvalidValue";
		}
	}

	testVol.set_dim(0, 0, 0);
	#pragma unroll
	for (uint8_t iDim = 0; iDim < 3; iDim++)
	{
		if (testVol.get_length(iDim) != 0.0)
		{
			printf("Length should be definitely 0 at the moment\n");
			throw "InvalidValue";
		}
	}

	testVol.set_dim(100, 101, 102);
	if (
		(testVol.get_length(0) != (100.0f * res[0])) || 
		(testVol.get_length(1) != (101.0f * res[1])) || 
		(testVol.get_length(2) != (102.0f * res[2])))
	{
		printf("Length looks invalid\n");
		printf("Values from class: %.1f, %.1f, %.1f\n", 
			testVol.get_length(0), testVol.get_length(1), testVol.get_length(2));
		printf("Values to test: %.1f, %.1f, %.1f\n", 
			100.0f * res[0], 101.0f * res[1], 102.0f * res[2]);
		throw "InvalidValue";
	}

	return 0;
}