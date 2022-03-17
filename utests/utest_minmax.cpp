/*
	A small helper script intended to check the calculation of the minimum and maximum value
	Author: Urs Hofmann
	Mail: mail@hofmannu.org
	Date: 17.03.2022
*/

#include "../src/volume.h"

int main()
{
	volume testVol(200, 200, 200);
	testVol.fill_rand(-1.0f, 2.0f);

	testVol[10] = 2.13f;
	testVol[11] = -1.25f;

	testVol.calcMinMax();

	if (testVol.get_minVal() != -1.25f)
	{
		printf("Procedure returned wrong minimum value\n");
		throw "InvalidValue";
	}

	if (testVol.get_maxVal() != 2.13f)
	{
		printf("Procedure returned wrong maximum value: %.2f instead of %.2f\n",
			testVol.get_maxVal(), 2.13f);
		throw "InvalidValue";
	}

	return 0;
}