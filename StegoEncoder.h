#pragma once 
#include "ArgumentList.h"
#include <vector>
#include <fstream>
#include <openssl/evp.h>
#include <openssl/md5.h>

#define MAX_CYPHERTEXT_SIZE 1024
namespace Crypto {
	class Encoder {
	public:
		Encoder(){}

		std::ostringstream Encrypt(std::istream& input, Config::ArgumentList &args)
		{
			std::ostringstream output(std::ios::binary);
			Cypher(input, output,  args.GetPassword(), args.GetEncryptionFunction(),  1);
			return output;
		}
		
		std::ofstream Decrypt(std::istream& input, Config::ArgumentList &args)
		{
			std::ofstream  output(args.GetOutFilePath(), std::ios_base::binary);
			if (!output.is_open()) {
				throw std::invalid_argument("Could not open output file");
			}
			Cypher(input, output, args.GetPassword(), args.GetEncryptionFunction(), 0);
			return output;
		}
		
		// mode = {1: encrypt, 0: decrypt, -1: do nothing}
		void Cypher(std::istream &input, std::ostream& output, std::string password, const EVP_CIPHER * encFunction, int mode)
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
			while (!input.eof()) {
				auto read = input.read(inputBlock, MAX_CYPHERTEXT_SIZE-1).gcount();
				EVP_CipherUpdate(ctx, reinterpret_cast<unsigned char *>(outputBlock), &outl, reinterpret_cast<unsigned char *>(inputBlock), static_cast<int>(read));
				output.write(outputBlock, outl);
			}
			EVP_CipherFinal(ctx, reinterpret_cast<unsigned char *>(outputBlock), &outl);
			output.write(outputBlock, outl);
			// Dont forget to free cypher context
			EVP_CIPHER_CTX_free(ctx);
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
			EVP_BytesToKey(encFunction, EVP_sha256(), nullptr, reinterpret_cast<const uint8_t*>(password.data()), static_cast<int>(password.size()), 1, key.get(), iv.get());
		}
	};
}