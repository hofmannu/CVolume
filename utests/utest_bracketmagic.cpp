/*
	utest_bracketmagic.cpp
	Author: Urs Hofmann
	Mail: mail@hofmannu.org
	Date: 16.03.2022
*/

#include "../src/volume.h"

int main()
{
	volume testVol(100, 100, 100);
	testVol.fill_rand();

	// see if brackets return the same thing as our get_value function
	if (testVol.get_value(20) != testVol[20])
	{
		printf("Bracket operator and get_value should behave the same");
		throw "InvalidValue";
	}

	// try to set a value with brackets
	testVol[10] = 0.1f;
	if (testVol[10] != 0.1f)
	{
		printf("Setting the value of index 10 did not work as expected\n");
		throw "InvalidValue";
	}

	// with round brackets we can get the elements from a 3d position
	float multiDimAccess = testVol(12, 13, 14);
	if (multiDimAccess != testVol.get_value(12, 13, 14))
	{
		printf("Returning a value with round bracket did not work as expected\n");
		throw "InvalidValue";
	}

	return 0;
}