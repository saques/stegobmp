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

			//	Derive a key and iv from the user given password
			std::unique_ptr<uint8_t[]> key, iv;
			DeriveKey(encFunction, password, key, iv);

			//	Start cyphering
			EVP_CipherInit(ctx, encFunction, key.get(), iv.get(), mode);

			// Cypher in MAX_CYPHERTEXT_SIZE chunks, complete cyphered message in output
			char inputBlock[MAX_CYPHERTEXT_SIZE], outputBlock[MAX_CYPHERTEXT_SIZE];
			int outl;
			std::string output;
			while (!input.eof()) {
				int read = input.read(inputBlock, MAX_CYPHERTEXT_SIZE).gcount();
				EVP_CipherUpdate(ctx, reinterpret_cast<unsigned char *>(outputBlock), &outl, reinterpret_cast<unsigned char *>(inputBlock), read);
				output.append(outputBlock, static_cast<size_t>(outl));
			}
			EVP_CipherFinal(ctx, reinterpret_cast<unsigned char *>(outputBlock), &outl);
			output.append(outputBlock, static_cast<size_t>(outl));
			// Dont forget to free cypher context
			EVP_CIPHER_CTX_free(ctx);
			return output;
		}

	private:
		void DeriveKey(const EVP_CIPHER * encFunction, std::string password, std::unique_ptr<uint8_t[]> &key, std::unique_ptr<uint8_t[]> &iv)
		{
			// count -> what iteration count should we use?
			int keyLength = EVP_CIPHER_key_length(encFunction);
			int ivLength = EVP_CIPHER_iv_length(encFunction);
			key = std::unique_ptr<uint8_t[]>(new uint8_t[keyLength]);
			iv = std::unique_ptr<uint8_t[]>(new uint8_t[ivLength]);
			// We use count = 1, the default
			EVP_BytesToKey(encFunction, EVP_md5(), nullptr, reinterpret_cast<const uint8_t*>(password.data()), password.size(), 1, key.get(), iv.get());
		}


		std::string slurp(std::ifstream& in) {
			std::stringstream sstr;
			sstr << in.rdbuf();
			return sstr.str();
		}
	};
}