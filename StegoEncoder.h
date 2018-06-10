#pragma once 
#include "ArgumentList.h"
#include <vector>
#include <fstream>
#include <openssl/evp.h>
#define MAX_CYPHERTEXT_SIZE 128
namespace Crypto {
	class Encoder {
	public:
		Encoder(){}

		std::string Encode(Config::ArgumentList &args)
		{
			std::ifstream input(args.GetInFilePath());
			if (!input.is_open()) {
				throw std::invalid_argument("Invalid path: Could not open the input file.");
			}

			EVP_CIPHER_CTX * ctx = EVP_CIPHER_CTX_new();
			EVP_EncryptInit(ctx, EVP_aes_128_cbc(), reinterpret_cast<const unsigned char *>(args.GetPassword().c_str()), nullptr);

			char plainText[MAX_CYPHERTEXT_SIZE];
			unsigned char cypherText[MAX_CYPHERTEXT_SIZE];
			int outl;
			std::string output;
			while (!input.eof()) {
				int read = input.read(plainText, MAX_CYPHERTEXT_SIZE).gcount();
				EVP_EncryptUpdate(ctx, cypherText, &outl, (unsigned char *) plainText, read);
				output.append(reinterpret_cast<const char *>(cypherText), static_cast<size_t>(outl));
			}
			EVP_EncryptFinal(ctx, cypherText, &outl);
			EVP_CIPHER_CTX_free(ctx);
			output.append(reinterpret_cast<const char *>(cypherText), static_cast<size_t>(outl));
			return output;
		}
		
		std::string Decode(Config::ArgumentList &args)
		{
			std::ifstream input(args.GetInFilePath());
			if (!input.is_open()) {
				throw std::invalid_argument("Invalid path: Could not open the input file.");
			}

			EVP_CIPHER_CTX ctx;
			EVP_CIPHER_CTX_init(&ctx);
			EVP_DecryptInit(&ctx, EVP_aes_128_cbc(), reinterpret_cast<const unsigned char *>(args.GetPassword().c_str()), nullptr);

			char plainText[MAX_CYPHERTEXT_SIZE];
			unsigned char cypherText[MAX_CYPHERTEXT_SIZE];
			int outl;
			std::string output;
			while (!input.eof()) {
				int read = input.read(plainText, MAX_CYPHERTEXT_SIZE).gcount();
				EVP_DecryptUpdate(&ctx, cypherText, &outl, (unsigned char *)plainText, read);
				output.append(reinterpret_cast<const char *>(cypherText), static_cast<size_t>(outl));
			}
			EVP_DecryptFinal(&ctx, cypherText, &outl);
			output.append(reinterpret_cast<const char *>(cypherText), static_cast<size_t>(outl));
			return output;
		}
	private:
		
		std::string slurp(std::ifstream& in) {
			std::stringstream sstr;
			sstr << in.rdbuf();
			return sstr.str();
		}
	};
}