#pragma once
#include <fstream>

namespace Config {
	enum class StegoAlgorithms { LSB1, LSB4, LSBE, UNDEFINED };

	std::istream& operator>>(std::istream& stream, StegoAlgorithms& algorithm) {
		std::string str;
		stream >> str;
		if (str == "LSB1") {
			algorithm = StegoAlgorithms::LSB1;
		}
		else if (str == "LSB4") {
			algorithm = StegoAlgorithms::LSB4;
		}
		else if (str == "LSBE") {
			algorithm = StegoAlgorithms::LSBE;
		}
		else {
			algorithm = StegoAlgorithms::UNDEFINED;
		}
		return stream;
	}

	enum class StegoCypher { DES, AES128, AES192, AES256, UNDEFINED };

	std::istream& operator>>(std::istream& stream, StegoCypher& cypher) {
		std::string str;
		stream >> str;
		if (str == "des") {
			cypher = StegoCypher::DES;
		}
		else if (str == "aes128") {
			cypher = StegoCypher::AES128;
		}
		else if (str == "aes192") {
			cypher = StegoCypher::AES192;
		}
		else if (str == "aes256") {
			cypher = StegoCypher::AES256;
		}
		else {
			cypher = StegoCypher::UNDEFINED;
		}
		return stream;
	}

	enum class StegoBlock { ECB, CFB, OFB, CBC, UNDEFINED };

	std::istream& operator>>(std::istream& stream, StegoBlock& block) {
		std::string str;
		stream >> str;
		if (str == "ecb") {
			block = StegoBlock::ECB;
		}
		else if (str == "cfb") {
			block = StegoBlock::CFB;
		}
		else if (str == "ofb") {
			block = StegoBlock::OFB;
		}
		else if (str == "cbc") {
			block = StegoBlock::CBC;
		}
		else {
			block = StegoBlock::UNDEFINED;
		}
		return stream;
	}
}