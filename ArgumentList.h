#pragma once
#include <cxxopts.hpp>

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
				("steg", "Algoritmo de esteganografiado", cxxopts::value<std::string>(steg)->default_value("LSB1"))
				("in", "El archivo de entrada", cxxopts::value<std::string>(inFilePath)->default_value(""))
				("a", "Algoritmo de cifrado", cxxopts::value<std::string>(a)->default_value("des"))
				("m", "Cifrado en bloque", cxxopts::value<std::string>(m)->default_value("ecb"))
				("pass", "Password de encriptacion", cxxopts::value<std::string>());
				
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
			if(inFilePath.empty()) {
				val += "Falta --in <archivo de entrada>.\n";
			}
			if (outFilePath.empty() && embed) {
				val += "Falta --out <archivo de salida>.\n";
			}
			if (steg != "LSB1" && steg != "LSB4" && steg != "LSBE") {
				val += "Falta --steg <LSB1|LSB4|LSBE>";
			}
			if (a != "des" && a != "aes128" && a != "aes192" && a != "aes256") {
				val += "Falta -a <des|aes128|aes192|aes256>";
			}
			if (m != "ecb" && m != "cfb" && m != "ofb" && m != "cbc") {
				val += "Falta -m <ecb|cfb|ofb|cbc>";
			}
			if (!val.empty()) {
				return val;
			}
			else {
				valid = true;
				return "";
			}
		}
		
	private:
		cxxopts::Options options;
		std::string inFilePath;
		std::string outFilePath;
		std::string carrierFilePath;
		std::string steg;
		std::string a;
		std::string m;
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