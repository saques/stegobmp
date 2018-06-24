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

	Structures::BMP file ("../images/ladoLSB4.bmp");

    /*
	std::string str = "Tam fortis, tamen tam stupidus! Utinam habeas cerebrum simile tuae fortitudini.";
	std::vector<uint8_t> data(str.begin(), str.end());

	file.Write(data, Config::StegoInsertion::LSB1);
    */

	auto diff = file.Read(Config::StegoInsertion::LSB4);

    /*
	for(auto it = diff.begin(); it != diff.end(); it++){
		std::cout << (char)*it;
	}
	std::cout << std::endl;
    */

	std::ofstream  output_file("example.pdf", std::ios_base::binary);
	std::ostream_iterator<std::uint8_t> output_iterator(output_file);
	std::copy(diff.begin(), diff.end(), output_iterator);



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