/*
	random number test for our volume class
	Author: Urs Hofmann
	Mail: mail@hofmannu.org
	Date: 26.02.2022

	Descriptiuon: a small unit test function to check random number generator

*/

#include "../src/volume.h"


int main()
{

	volume volIn(100, 100, 100);


	// here we fill with range -1 ... 1
	volIn.fill_rand(-1.0f, 1.0f);
	volIn.calcMinMax();
	if (volIn.get_minVal() < -1.0f)
	{
		printf("Minimum value returned should be higher or equal -1.0f\n");
		throw "InvalidValue";
	}

	if (volIn.get_maxVal() > 1.0f)
	{
		printf("Maximumg value returned should be smaller or equal 1.0f");
	}


	// here we fill with range 0 ... 2
	volIn.fill_rand(2.0f);
	volIn.calcMinMax();
	if (volIn.get_minVal() < 0.0f)
	{
		printf("Minimum value returned should be higher or equal -1.0f\n");
		throw "InvalidValue";
	}

	if (volIn.get_maxVal() > 2.0f)
	{
		printf("Maximumg value returned should be smaller or equal 1.0f");
	}

	// now we fill in range 0 ... 1
	volIn.fill_rand();
	volIn.calcMinMax();
	if (volIn.get_minVal() < 0.0f)
	{
		printf("Minimum value returned should be higher or equal -1.0f\n");
		throw "InvalidValue";
	}

	if (volIn.get_maxVal() > 1.0f)
	{
		printf("Maximumg value returned should be smaller or equal 1.0f");
	}




	return 0;
}