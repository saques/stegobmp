#include <cstdlib>
#include <cstring>
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

    structures::BMP bmp ("../images/land1.bmp");

    /**
     * An example: altering the least significant bit every 10 pixels.
     */

    for(int x = 0; x < 1024 ; x+=10){
    	for(int y = 0; y < 768; y+=10){
    		uint64_t p = bmp.read(x,y);
    		bmp.write(x, y, p^1);
    	}
    }


    bmp.save("other1.bmp");

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