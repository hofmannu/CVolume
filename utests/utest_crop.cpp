#include "../src/volume.h"

int main()
{
	volume testVol(101, 99, 100);
	volume testVolCpy;
	testVolCpy = testVol;

	uint64_t startIdx[3] = {11, 12, 13};
	uint64_t stopIdx[3] = {90, 90, 90};

	testVol.crop(startIdx, stopIdx);

	for (uint8_t iDim = 0; iDim < 3; iDim++)
	{
		const uint64_t newDim = stopIdx[iDim] - startIdx[iDim] + 1;
		if (testVol.get_dim(iDim) != newDim)
		{
			printf("cropping let to incorrect dimensions along dim %d (%lu instead of %lu)\n", 
				iDim, testVol.get_dim(iDim), newDim);
			throw "InvalidValue";
		}
	}

	// try cropping on copy (should work just fine)
	testVolCpy.crop(startIdx, stopIdx);

	for (uint8_t iDim = 0; iDim < 3; iDim++)
	{
		const uint64_t newDim = stopIdx[iDim] - startIdx[iDim] + 1;
		if (testVolCpy.get_dim(iDim) != newDim)
		{
			printf("cropping let to incorrect dimensions along dim %d (%lu instead of %lu)\n", 
				iDim, testVolCpy.get_dim(iDim), newDim);
			throw "InvalidValue";
		}
	}

	return 0;
}