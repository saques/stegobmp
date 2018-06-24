#include <stdlib.h>
#include <string>
#include <iostream>
#include <openssl/md5.h>
#include <openssl/evp.h>
#include <cxxopts.hpp>
#include <easylogging++.h>
#include "ArgumentList.h"
#include "BMP.h"
#include "StegoEncoder.h"

INITIALIZE_EASYLOGGINGPP


int extract(Config::ArgumentList& opts)
{
	Structures::BMP carrier(opts.GetCarrierFilePath());
	auto encryptedMessage = carrier.Read(opts.GetStegoInsertion());
	auto encryptedStringStream = std::stringstream(std::string(encryptedMessage.begin(), encryptedMessage.end()));
	Crypto::Encoder encoder;
	auto outputFile = encoder.Decrypt(encryptedStringStream, opts);
	return EXIT_SUCCESS;
}

int embed(Config::ArgumentList& opts)
{
	std::ifstream plainText(opts.GetInFilePath(), std::ios_base::binary);
	if (!plainText.is_open()) {
		throw std::invalid_argument("Could not open input file.");
	}
	Crypto::Encoder encoder;
	auto message = encoder.Encrypt(plainText, opts);
	Structures::BMP carrier(opts.GetCarrierFilePath());
	auto messageStr = message.str();
	std::vector<uint8_t> vectorMessage(messageStr.begin(), messageStr.end());
	carrier.Write(vectorMessage, opts.GetStegoInsertion());
	carrier.Save(opts.GetOutFilePath());
	return EXIT_SUCCESS;
}


int stegobmp(Config::ArgumentList& opts)
{
	bool valid;
	auto err = opts.OptionsAreValid(valid);
	if (!valid) {
		std::cout << err << std::endl;
		return EXIT_FAILURE;
	}

	if (opts.IsExtract()) {
		return extract(opts);
	}
	else {
		return embed(opts);
	}
}

int main(int argc, char*argv[])
{	

	//Structures::BMP file ("../images/ladoLSB4.bmp");
    /*
	std::string str = "Tam fortis, tamen tam stupidus! Utinam habeas cerebrum simile tuae fortitudini.";
	std::vector<uint8_t> data(str.begin(), str.end());

	file.Write(data, Config::StegoInsertion::LSB1);
    */

	//auto diff = file.Read(Config::StegoInsertion::LSB4);

    /*
	for(auto it = diff.begin(); it != diff.end(); it++){
		std::cout << (char)*it;
	}
	std::cout << std::endl;
    */

	//std::ofstream  output_file("example.pdf", std::ios_base::binary);
	//std::ostream_iterator<std::uint8_t> output_iterator(output_file);



	try {
		Config::ArgumentList opts(argc, argv);
		return stegobmp(opts);
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
}