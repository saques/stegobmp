#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <openssl/md5.h>
#include <openssl/evp.h>
#include <cxxopts.hpp>
#include <easylogging++.h>
#include "ArgumentList.h"

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
	try {
		Config::ArgumentList opts(argc, argv);
		return stegobmp(opts);
	}
	catch (cxxopts::OptionException& ex) {
		std::cout << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
}