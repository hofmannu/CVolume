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



	return 0;
}