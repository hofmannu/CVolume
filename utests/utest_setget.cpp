#include "../src/volume.h"

int main()
{
	volume testVol;

	testVol.set_dim(10, 11, 12);
	if ((testVol.get_dim(0) != 10) || (testVol.get_dim(1) != 11) || (testVol.get_dim(2) != 12))
	{
		printf("Setting or getting dimensions seems fishy\n");
		throw "InvalidValue";
	}	

	uint64_t startVal = 123;
	for (uint8_t iDim = 0; iDim < 3; iDim++)
	{
		testVol.set_dim(iDim, startVal);
		if (testVol.get_dim(iDim) != startVal)
		{
			printf("Setting or getting dimensions seems fishy\n");
			throw "InvalidValue";
		}
		startVal++;
	}

	if ((testVol.get_dim(0) != 123) || (testVol.get_dim(1) != 124) || (testVol.get_dim(2) != 125))
	{
		printf("Setting or getting dimensions seems fishy\n");
		throw "InvalidValue";
	}	

	const float res0 = 0.1;
	const float res1 = res0 * 3;
	const float res2 = res0 + res1;
	testVol.set_res(res0, res1, res2);
	if ((testVol.get_res(2) != res2) || (testVol.get_res(1) != res1) || (testVol.get_res(0) != res0))
	{
		printf("Setting or getting resolution seems fishy: %.7f, %.7f, %.7f\n", 
			testVol.get_res(0), testVol.get_res(1), testVol.get_res(2));
		throw "InvalidValue";
	}	

	if (res0 != testVol.get_res0())
	{
		printf("Cannot retrieve correct value for resolution along 0\n");
		throw "InvalidValue";
	}

	if (res1 != testVol.get_res1())
	{
		printf("Cannot retrieve correct value for resolution along 1\n");
		throw "InvalidValue";	
	}

	if (res2 != testVol.get_res2())
	{
		printf("Cannot retrieve correct value for resolution along 2\n");
		throw "InvalidValue";	
	}

	const float origin[3] = {0.11, 0.23, 0.456};
	testVol.set_origin(origin[0], origin[1], origin[2]);
	if (
		(origin[0] != testVol.get_origin(0)) || 
		(origin[1] != testVol.get_origin(1)) || 
		(origin[2] != testVol.get_origin(2)))
	{
		printf("Could not retrieve correct origin for dataset\n");
		throw "InvalidValue";
	}

	if (origin[0] != testVol.get_origin(0))
	{
		printf("Could not retrieve correct origin at dim0\n");
		throw "InvalidValue";
	}

	if (origin[1] != testVol.get_origin(1))
	{
		printf("Could not retrieve correct origin at dim1\n");
		throw "InvalidValue";
	}

	if (origin[2] != testVol.get_origin(2))
	{
		printf("Could not retrieve correct origin at dim2\n");
		throw "InvalidValue";
	}

	return 0;
}