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


	/**
	 *
    std::vector<uint8_t> vector;
	std::istream_iterator<uint8_t> istream_iterator(plainText);
    std::copy( std::istream_iterator<uint8_t>(istream_iterator),
               std::istream_iterator<uint8_t>(),
               std::back_inserter(vector));

    uint64_t size = vector.size();
	//Write size
    for(int p=3; p>=0; p--){
        vector.insert(vector.begin(),(uint8_t)(size>>(uint8_t)((3-p)*8)));
    }

	//Write fmt
    vector.insert(vector.end(), (uint8_t)".");
	for(auto it = opts.GetInFileExtension().begin(); it != opts.GetInFileExtension().end(); ++it)
		vector.insert(vector.end(), *it);

	std::ifstream payload;
	std::copy(vector.begin(), vector.end(),
		 std::istream_iterator<uint8_t >(payload));

	 */



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