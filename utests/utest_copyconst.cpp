/*
	copy constructor test for our volume class
	Author: Urs Hofmann
	Mail: mail@hofmannu.org
	Date: 26.02.2022

	Descriptiuon: a small unit test function for the copy constructor of our class

*/

#include "../src/volume.h"

int main()
{

	volume volIn(100, 100, 100);
	volIn.fill_rand();
	
	volume volOut = volIn;
	if (volOut != volIn)
	{
		printf("Copy constructor should generate identical volumes");
		throw "InvalidValue";
	}

	// check if volumes are the same
	if (volOut != volIn)
	{
		printf("Copy constructor should result in the same elements for entire volume");
		throw "InvalidValue";
	}

	// set a value in volOut and make sure that volIn remains the same
	const float testVal = volIn.get_value(101);
	volOut.set_value(101, 1.1f);
	if (testVal != volIn.get_value(101))
	{
		printf("A modification to the copied volume should leave input volume intact.\n");
		throw "InvalidValue";
	}

	volOut = volIn;

	return 0;
}