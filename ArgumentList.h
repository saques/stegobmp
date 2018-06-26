#pragma once
#include <cxxopts.hpp>
#include <openssl/evp.h>
#include "StegoEnums.h"
#include <functional>
namespace Config 
{
	class ArgumentList {
	public:
		ArgumentList(int argc, char ** argv) :
			options("stegobmp", "BMP stenography.")
		{
			options.add_options()
				("embed", "Indica que se va a ocultar informacion", cxxopts::value(embed))
				("extract", "Indica que se va a extraer informacion", cxxopts::value(extract))
				("h,help", "Mostrar commandos.", cxxopts::value(help))
				("p", "Archivo bmp que sera el portador", cxxopts::value<std::string>(carrierFilePath)->default_value(""))
				("out", "Archivo de salida", cxxopts::value<std::string>(outFilePath)->default_value(""))
				("steg", "Algoritmo de esteganografiado", cxxopts::value<StegoInsertion>(steg)->default_value("LSB1"))
				("in", "El archivo de entrada", cxxopts::value<std::string>(inFilePath)->default_value(""))
				("a", "Algoritmo de cifrado", cxxopts::value<StegoCypher>(cypher)->default_value("undefined"))
				("m", "Cifrado en bloque", cxxopts::value<StegoBlock>(block)->default_value("undefined"))
				("pass", "Password de encriptacion", cxxopts::value<std::string>(password));
				
			options.custom_help("[--embed|--extract] [OPTION...] ");
			options.parse(argc, argv);

		};

		/*	Checks if arguments make up a valid request.
			Command line cannot have:
				* There is and -h or --help argument.
				* Both an -embed and -extract arguments.
		*/
		std::string OptionsAreValid(bool& valid)
		{
			valid = false;
			if (help) {
				return GetHelp();
			}
			if (!(!(embed && extract) && (embed || extract))) {
				return "Falta --embed o --extract.";
			}
			std::string val;
			if (carrierFilePath.empty()) {
				val += "Falta -p <archivo bmp portador>.\n";
			}
			if(inFilePath.empty() && embed) {
				val += "Falta --in <archivo de entrada>.\n";
			}
			if (outFilePath.empty()) {
				val += "Falta --out <archivo de salida>.\n";
			}
			if (steg == StegoInsertion::UNDEFINED) {
				val += "Falta --steg <LSB1|LSB4|LSBE>. \n";
			}
			if (!password.empty()) {	
				if (cypher == StegoCypher::UNDEFINED) {
					cypher = StegoCypher::AES128;
				}
				if (block == StegoBlock::UNDEFINED) {
					block = StegoBlock::CBC;
				}
			}
			else {
				if (cypher != StegoCypher::UNDEFINED || block != StegoBlock::UNDEFINED) {
					val += "No se puede encriptar sin especificar una clave.\n";
				}
			}
			
			if (!val.empty()) {
				return val;
			}
			else {
				valid = true;
				return "";
			}
		}
		
		const std::string& GetInFilePath() 
		{
			return inFilePath;
		}

		const std::vector<uint8_t>& GetInFileExtension()
		{
			return inFileExtension;
		}

		const StegoInsertion& GetStegoInsertion() 
		{
			return steg;
		}

		const StegoCypher& GetCypher()
		{
			return cypher;
		}

		const StegoBlock& GetBlock()
		{
			return block;
		}

		const std::string& GetOutFilePath()
		{
			return outFilePath;
		}

		const std::string& GetCarrierFilePath()
		{
			return carrierFilePath;
		}

		const std::string& GetPassword()
		{
			return password;
		}

		bool IsEmbed()
		{
			return embed;
		}

		bool IsExtract()
		{
			return extract;
		}

		const EVP_CIPHER* GetEncryptionFunction()
		{
			switch (cypher) 
			{
			case StegoCypher::AES128: 
				switch (block)
				{
				case StegoBlock::CBC:
					return EVP_aes_128_cbc();
				case StegoBlock::CFB:
					return EVP_aes_128_cfb8();
				case StegoBlock::ECB:
					return EVP_aes_128_ecb();
				case StegoBlock::OFB:
					return EVP_aes_128_ofb();
				default:
					return EVP_enc_null();
				}
			case StegoCypher::AES192: 
				switch (block)
				{
				case StegoBlock::CBC:
					return EVP_aes_192_cbc();
				case StegoBlock::CFB:
					return EVP_aes_192_cfb8();
				case StegoBlock::ECB:
					return EVP_aes_192_ecb();
				case StegoBlock::OFB:
					return EVP_aes_192_ofb();
				default:
					return EVP_enc_null();
				}

			case StegoCypher::AES256: 
				switch (block)
				{
				case StegoBlock::CBC:
					return EVP_aes_256_cbc();
				case StegoBlock::CFB:
					return EVP_aes_256_cfb8();
				case StegoBlock::ECB:
					return EVP_aes_256_ecb();
				case StegoBlock::OFB:
					return EVP_aes_256_ofb();
				default:
					return EVP_enc_null();
				}

			case StegoCypher::DES: 
				switch (block)
				{
				case StegoBlock::CBC:
					return EVP_des_cbc();
				case StegoBlock::CFB:
					return EVP_des_cfb8();
				case StegoBlock::ECB:
					return EVP_des_ecb();
				case StegoBlock::OFB:
					return EVP_des_ofb();
				default:
					return EVP_enc_null();
				}
			default: 
			{
				return EVP_enc_null();
			}
			}
		}

	private:
		cxxopts::Options options;
		std::string inFilePath;
		std::vector<uint8_t> inFileExtension;
		std::string outFilePath;
		std::string carrierFilePath;
		StegoInsertion steg;
		StegoCypher cypher;
		StegoBlock block;
		std::string password;
		bool embed;
		bool extract;
		bool help;

		std::string GetHelp()
		{
			return options.help({ "", "embed", "extract" });
		}
	};


}