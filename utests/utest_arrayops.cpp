#include "../src/volume.h"

int main()
{

	const uint64_t dimVol[3] = {100, 100, 100};

	volume testVolA(dimVol[0], dimVol[1], dimVol[2]);
	volume testVolB(dimVol[0], dimVol[1], dimVol[2]);

	// set entire array to 1.4
	testVolA = 1.4f;
	for (uint64_t iElement = 0; iElement < (dimVol[0] * dimVol[1] * dimVol[2]); iElement++)
	{
		if (testVolA.get_value(iElement) != 1.4f)
		{
			printf("Something went wrong while defining the volume\n");
			throw "InvalidValue";
		}

		if (testVolA[iElement] != 1.4f)
		{
			printf("Something went wrong while defining the volume\n");
			throw "InvalidValue";
		}
	}

	testVolB = testVolA;
	for (uint64_t iElement = 0; iElement < (dimVol[0] * dimVol[1] * dimVol[2]); iElement++)
	{
		if (testVolB.get_value(iElement) != 1.4f)
		{
			printf("Something went wrong while defining the volume\n");
			throw "InvalidValue";
		}
	}

	testVolB *= 3.0f; 

	for (uint64_t iElement = 0; iElement < (dimVol[0] * dimVol[1] * dimVol[2]); iElement++)
	{
		if (testVolB.get_value(iElement) != (1.4f * 3.0f))
		{
			printf("Something went wrong while defining the volume\n");
			throw "InvalidValue";
		}
	}

	// testing the following: A = B * c
	// where A is a volume, B is a volume and c is a constant
	// B needs to remain unchanged after operation
	volume A(100, 100, 100);
	volume B(100, 100, 100);
	A = 2.0f;
	B = 3.0f;
	A = B; A *= 4.0f;
	if (A[100] != 12.0f)
	{
		printf("A was not updated properly (A = B * c)\n");
		throw "InvalidValue";
	}
	
	if (B[100] != 3.0f)
	{
		printf("B should not be modified by this operation (A = B * c)\n");
		throw "InvalidValue";
	}


	// testing the following: A = B / c
	// where A is a volume, B is a volume and c is a constant
	// B needs to remain unchanged after operation
	A = 7.0f;
	B = 8.0f;
	A = B; A /= 2.0f;
	if (A[100] != 4.0f)
	{
		printf("A was not updated properly (A = B * c)\n");
		throw "InvalidValue";
	}
	
	if (B[100] != 8.0f)
	{
		printf("B should not be modified by this operation (A = B * c)\n");
		throw "InvalidValue";
	}

	// testing the following: A += B
	// where A is a volume, B is a volume
	// B needs to remain unchanged after operation
	A = 3.1f;
	B = 22.3f;
	A += B;
	if (A[234] != (3.1f + 22.3f))
	{
		printf("A was not updated properly (A = B * c)\n");
		throw "InvalidValue";
	}
	
	if (B[234] != 22.3f)
	{
		printf("B should not be modified by this operation (A = B * c)\n");
		throw "InvalidValue";
	}

	// testing the following: A -= B
	// where A is a volume, B is a volume
	// B needs to remain unchanged after operation
	A = 32.1f;
	B = 22.32f;
	A -= B;
	if (A[234] != (32.1f - 22.32f))
	{
		printf("A was not updated properly (A = B * c)\n");
		throw "InvalidValue";
	}
	
	if (B[234] != 22.32f)
	{
		printf("B should not be modified by this operation (A = B * c)\n");
		throw "InvalidValue";
	}

	A = 1.23f;
	B = 1.33f;
	if (A == B)
	{
		printf("Values should just not be the same now\n");
		throw "InvalidValue";
	}

	if (!(A != B))
	{
		printf("Values should just not be the same now\n");
		throw "InvalidValue";
	}

	A = 1.24f;
	B = 1.24f;
	if (!(A == B))
	{
		printf("Values should just be the same now\n");
		throw "InvalidValue";
	}

	if (A != B)
	{
		printf("Values should just not be the same now\n");
		throw "InvalidValue";
	}	printf("Looks like we passed all tests successfully so far\n");

	return 0;
}