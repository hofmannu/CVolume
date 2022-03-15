/*
	Tests the multiplication operation of our array
	Author: Urs Hofmann
	Mail: mail@hofmannu.org
	Date: 15.02.2022
*/

#include "../src/volume.h"

int main()
{
	std::size_t nElements = 110 * 110 * 110;
	volume volA(110, 110, 110);
	volA.fill_rand(-1.0f, 1.0f);
	volume volB(100, 100, 100);
	volB.fill_rand(-1.0f, 1.0f);

	// test relative multiplication operator
	float backup = volB[10];
	volB *= 2.0f;
	if ((backup * 2.0f) != volB[10])
	{
		printf("something went wrong during shallow multiplication\n");
		throw "InvalidValue";
	}

	volB = 0;

	// construct new volume based on multiplication
	volume volZ = volA * 2.0f;
	bool isSame = 1;
	for (std::size_t idx = 0; idx < nElements; idx++)
	{
	 	if (volZ[idx] != (volA[idx] * 2.0f))
			isSame = 0;
	}

	if (!isSame)
	{
		printf("Multiplication using operator somehow went wrong\n");
		throw "InvalidValue";
	}

	// test multiplication using simple assignment operator
	volB = volA * 2.345f;
	for (std::size_t idx = 0; idx < nElements; idx++)
	{
	 	if (volB[idx] != (volA[idx] * 2.345f))
			isSame = 0;
	}

	if (!isSame)
	{
		printf("Multiplication after assignment operator somehow went wrong\n");
		throw "InvalidValue";
	}


	return 0;
}