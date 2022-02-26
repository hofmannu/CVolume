#include "../src/volume.h"

int main()
{
	volume testVol(100, 100, 100);
	testVol = 0;
	testVol.set_value(100, 2.0f);

	testVol.normalize();
	
	if (testVol.get_value(100) != 1.0f)
	{
		printf("Normalization should result in 1 in this case\n");
		throw "InvalidValue";
	}

	printf("Normalization seems to work\n");

	testVol = 0;
	testVol.set_value(100, 2.0f);
	testVol.set_value(101, 2.0f);
	testVol.normalize();
	
	if (testVol.get_value(100) != 2.0f / (sqrtf(2.0f * 2.0f + 2.0f * 2.0f)))
	{
		printf("Normalization should result in 1 in this case\n");
		throw "InvalidValue";
	}

	return 0;
}