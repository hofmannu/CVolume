


// function takes two arguments, one is the input path to load from and the other
// is the output path to where we want to store the converted image

#include "volume.h"
#include <filesystem>

int main(int argc, char *argv[]) {

	// make sure the correct number of arguments are passed
	if (argc != 3) {
		std::cout << "Please pass two arguments to this function, first is input "
		"path, second is output path" << std::endl;
		return 1;
	}

	// parse arguments and print some information
	const std::string inputPath = argv[1];
	const std::string outputPath = argv[2];
	std::cout << "CVolume conversion tool" << std::endl
		<< " - input path: " << inputPath << std::endl
		<< " - output path: " << outputPath << std::endl;

	// check that input path is atually pointing to a valid file
	if (!std::filesystem::exists(inputPath)) {
		std::cout << "Input path is not pointing to a valid file: " << 
		inputPath << std::endl;
		return 1;
	}

	volume vol;
	vol.readFromFile(inputPath);
	vol.saveToFile(outputPath);


	return 0;
}