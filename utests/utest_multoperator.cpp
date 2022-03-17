/*
	Tests the multiplication operation of our array, representative for all operations
	Author: Urs Hofmann
	Mail: mail@hofmannu.org
	Date: 15.02.2022
*/

#include "../src/volume.h"
#include <chrono>

int main()
{
	std::size_t nX = 200;
	std::size_t nY = 210;
	std::size_t nZ = 220;

	std::size_t nElements = nX * nY * nZ;
	volume volA(nX, nY, nZ);
	volA.fill_rand(-1.0f, 1.0f);
	volume volB(nX, nY, nZ);
	volB.fill_rand(-1.0f, 1.0f);

	// test relative multiplication operator
	float backup = volB[10];

	const auto tStart = std::chrono::high_resolution_clock::now();
	volB *= 2.0f;
	const auto tStop = std::chrono::high_resolution_clock::now();
	
	if ((backup * 2.0f) != volB[10])
	{
		printf("something went wrong during shallow multiplication\n");
		throw "InvalidValue";
	}
	
	const float tDuration = std::chrono::duration_cast<std::chrono::milliseconds>(tStop - tStart).count();
	printf("Execution time was %.2f ms\n", tDuration);

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

	// multiply two volumes with each other and assign to third
	volZ = volA * volB;
	// check for all elements if things worked out
	for (std::size_t idx = 0; idx < nElements; idx++)
	{
		if (volZ[idx] != (volA[idx] * volB[idx]))
		{
			isSame = 0;
		}
	}

	if (!isSame)
	{
		printf("Multiplication of two volumes with each other did not work out\n");
		throw "InvalidValue";
	}


	return 0;
}