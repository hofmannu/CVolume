/*
	Tests the multiplication operation of our array
	Author: Urs Hofmann
	Mail: mail@hofmannu.org
	Date: 15.02.2022
*/

#include "../src/volume.h"

int main()
{
	int nElements = 100 * 100 * 100;
	volume volA(100, 100, 100);
	volA.fill_rand(-1.0f, 1.0f);
	volume volB(100, 100, 100);
	volB.fill_rand(-1.0f, 1.0f);

	bool isSame;

	volB *= 2.0f;

	// construct new volume based on multiplication
	// volB = volA * 2.0f;
	// isSame = 1;
	// for (int idx = 0; idx < nElements; idx++)
	// {
	// 	if (volB[idx] != (volA[idx] * 2.0f))
	// 		isSame = 0;
	// }
	// if (!isSame)
	// {
	// 	printf("Multiplication using operator somehow went wrong\n");
	// 	throw "InvalidValue";
	// }


	return 0;
}