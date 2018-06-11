#include <stdlib.h>
#include <string>
#include <iostream>
#include <openssl/md5.h>
#include <openssl/evp.h>
#include <cxxopts.hpp>
#include <easylogging++.h>
#include "ArgumentList.h"
#include "BMP.h"

INITIALIZE_EASYLOGGINGPP

int stegobmp(Config::ArgumentList& opts)
{
	bool valid;
	auto err = opts.OptionsAreValid(valid);
	if (!valid) {
		std::cout << err << std::endl;
		return EXIT_SUCCESS;
	}

	return EXIT_SUCCESS;
}

int main(int argc, char*argv[])
{	
    Structures::BMP original ("../images/lima.bmp");
	Structures::BMP altered ("../images/lima.bmp");

	std::string str = "Mauricio Macri la puta que te pario";
	std::vector<uint8_t> data(str.begin(), str.end());

	altered.Write(data, Config::StegoInsertion::LSB1);

	std::vector<uint8_t> diff = Structures::BMP::Diff(original, altered, Config::StegoInsertion::LSB1);

	for(auto it = diff.begin(); it != diff.end(); it++){
		std::cout << (char)*it;
	}

	std::cout << std::endl;

	altered.Save("altered.bmp");



    /*
	std::cout << "hfasdfasdsdf!" << std::endl;

	auto arr = "hello world";
	char * arr2 = new char[3];
	std::strcpy(arr2, arr);

	std::cout << "arr2" << arr2 << std::endl;
	*/

    /*
	try {
		Config::ArgumentList opts(argc, argv);
		return stegobmp(opts);
	}
	catch (cxxopts::OptionException& ex) {
		std::cout << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
    */
}