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
#include "Endiannes.h"
#include "StegoEnums.h"

INITIALIZE_EASYLOGGINGPP


int extract(Config::ArgumentList& opts)
{
	Structures::BMP carrier(opts.GetCarrierFilePath());
	uint32_t firstReadSize = 0;
	auto encryptedMessage = carrier.Read(opts.GetStegoInsertion(), opts.GetCypher(), firstReadSize);
	auto encryptedStringStream = std::stringstream(std::string(encryptedMessage.begin(), encryptedMessage.end()));
	Crypto::Encoder encoder;
	auto outString = encoder.Decrypt(encryptedStringStream, opts);

	// Read 4 bytes for size (BIG ENDIAN)
	uint32_t size;
	if (opts.GetCypher() != Config::StegoCypher::UNDEFINED) {
		outString.read(reinterpret_cast<char*>(&size), sizeof(size));
		Endiannes::SwapEndiannes(size);
	}
	else {
		size = firstReadSize;
	}
	
	// Check size makes sense
	if (size > encryptedMessage.size()) {
		throw std::exception("Read failed: reported size mismatch.");
	}

	// Read content
	std::string content;
	content.resize(size);
	outString.read(&content[0], size);

	// Read null terminated extension
	std::string extension;
	outString >> extension;
	
	// Create outputFile
	std::ofstream outputFile(opts.GetOutFilePath() + extension, std::ios::binary);

	// Just copy content 
	outputFile << content;
	return EXIT_SUCCESS;
}

std::streampos fileSize(std::ifstream& file) {

	std::streampos fsize = 0;
	fsize = file.tellg();
	file.seekg(0, std::ios::end);
	fsize = file.tellg() - fsize;
	file.clear();
	file.seekg(0, std::ios::beg);
	return fsize;
}



int embed(Config::ArgumentList& opts)
{
	std::ifstream plainText(opts.GetInFilePath(), std::ios::binary );
	if (!plainText.is_open()) {
		throw std::invalid_argument("Could not open input file.");
	}
	std::stringstream plainTextPlusSize;
	{
		// Insert content size (BIG ENDIAN)
		uint32_t size = fileSize(plainText);
		Endiannes::SwapEndiannes(size);
		plainTextPlusSize.write(reinterpret_cast<char*>(&size), sizeof(size));

		// Insert content
		plainTextPlusSize << plainText.rdbuf();

		// Insert extension
		std::string extension(opts.GetInFilePath().substr(opts.GetInFilePath().find_last_of(".")));
		plainTextPlusSize << extension + '\0';
	}
	Crypto::Encoder encoder;
	auto message = encoder.Encrypt(plainTextPlusSize, opts);
	Structures::BMP carrier(opts.GetCarrierFilePath());
	auto messageStr = message.str();
	std::vector<uint8_t> vectorMessage(messageStr.begin(), messageStr.end());
	carrier.Write(vectorMessage, opts.GetStegoInsertion(), opts.GetCypher());
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
	try {
		Config::ArgumentList opts(argc, argv);
		return stegobmp(opts);
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
}