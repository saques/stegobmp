#pragma once 
#include "ArgumentList.h"
#include <vector>
#include <fstream>
#include <openssl/evp.h>
#include <openssl/md5.h>

#define MAX_CYPHERTEXT_SIZE 128
namespace Crypto {
	class Encoder {
	public:
		Encoder(){}

		std::string Encrypt(Config::ArgumentList &args)
		{
			std::ifstream input(args.GetInFilePath());
			if (!input.is_open()) {
				throw std::invalid_argument("Invalid path: Could not open the input file.");
			}
			return Cypher(input, args.GetPassword(), args.GetEncryptionFunction(),  1);
		}
		
		std::string Decrypt(Config::ArgumentList &args)
		{
			std::ifstream input(args.GetInFilePath());
			if (!input.is_open()) {
				throw std::invalid_argument("Invalid path: Could not open the input file.");
			}
			return Cypher(input, args.GetPassword(), args.GetEncryptionFunction(), 0);
		}
		
		// mode = {1: encrypt, 0: decrypt, -1: do nothing}
		std::string Cypher(std::istream &input, std::string password, const EVP_CIPHER * encFunction, int mode)
		{
			//	Initialize EVP Cypher context
			EVP_CIPHER_CTX * ctx = EVP_CIPHER_CTX_new();

			//	Derive a MD5 key from the user given password
			unsigned char key[16];
			DeriveKey(password, key);

			//	Start cyphering
			//	Should add an IV? If so, add a new parameter?
			EVP_CipherInit(ctx, encFunction, key, nullptr, mode);

			// Cypher in MAX_CYPHERTEXT_SIZE chunks, complete cyphered message in output
			char inputBlock[MAX_CYPHERTEXT_SIZE];
			unsigned char outputBlock[MAX_CYPHERTEXT_SIZE];
			int outl;
			std::string output;
			while (!input.eof()) {
				int read = input.read(inputBlock, MAX_CYPHERTEXT_SIZE).gcount();
				EVP_CipherUpdate(ctx, outputBlock, &outl, reinterpret_cast<unsigned char *>(inputBlock), read);
				output.append(reinterpret_cast<const char *>(outputBlock), static_cast<size_t>(outl));
			}
			EVP_CipherFinal(ctx, outputBlock, &outl);
			output.append(reinterpret_cast<const char *>(outputBlock), static_cast<size_t>(outl));
			// Dont forget to free cypher context
			EVP_CIPHER_CTX_free(ctx);
			return output;
		}

	private:
		void DeriveKey(std::string password, unsigned char key[16])
		{
			key = MD5(reinterpret_cast<const unsigned char *>(password.c_str()), password.size(), key);
		}


		std::string slurp(std::ifstream& in) {
			std::stringstream sstr;
			sstr << in.rdbuf();
			return sstr.str();
		}
	};
}