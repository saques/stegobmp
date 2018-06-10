#include <gtest/gtest.h>
#include "ArgumentList.h"
#include "StegoEnums.h"
#include <functional>
#include "StegoEncoder.h"

static Config::ArgumentList SetUpArgcArgv(std::vector<char *> args)
{
	Config::ArgumentList opts((int)args.size(), &args[0]);
	return opts;
}

template<class T>
static void TestOptions(std::function<T(void)> f, T expectedValue)
{
	EXPECT_EQ(f(), expectedValue);
}

template<class T>
void TestParam(char * commandLineOption, char * arg, T stego, const T &(Config::ArgumentList::*getter)(void))
{
	auto opts = SetUpArgcArgv(std::vector<char *> {"test", commandLineOption, arg});
	TestOptions<T>(std::bind(getter, opts), stego);
}

TEST(ArgumentList, ArgumentsWorkProperly) {
	TestParam("--steg", "LSB1", Config::StegoInsertion::LSB1, &Config::ArgumentList::GetStego);
	TestParam("--steg", "LSB4", Config::StegoInsertion::LSB4, &Config::ArgumentList::GetStego);
	TestParam("--steg", "asfdas", Config::StegoInsertion::UNDEFINED, &Config::ArgumentList::GetStego);
	
	TestParam("-a", "aes128", Config::StegoCypher::AES128, &Config::ArgumentList::GetCypher);
	TestParam("-a", "aes192", Config::StegoCypher::AES192, &Config::ArgumentList::GetCypher);
	TestParam("-a", "whatever", Config::StegoCypher::UNDEFINED, &Config::ArgumentList::GetCypher);

	TestParam("-m", "ecb", Config::StegoBlock::ECB, &Config::ArgumentList::GetBlock);
	TestParam("-m", "cbc", Config::StegoBlock::CBC, &Config::ArgumentList::GetBlock);

	TestParam("--pass", "holamundo", std::string("holamundo"), &Config::ArgumentList::GetPassword);
}

void TestAreOptionsValid(std::vector<char *> args, bool expected)
{
	auto opts = SetUpArgcArgv(args);
	bool valid;
	opts.OptionsAreValid(valid);
	EXPECT_EQ(valid, expected);
}

std::vector<char*> ConcatVectors(const std::vector<char*> &vect1, const std::vector<char*> &vect2)
{
	std::vector<char*> ret(vect1);
	ret.insert(ret.end(), vect2.begin(), vect2.end());
	return ret;
}

TEST(ArgumentList, AreOptionsValid) {

	std::vector<char *> defaultArgs {
		"test",
		"--embed",
		"--in",
		"test.txt",
		"-p",
		"test.txt",
		"--out",
		"test.txt",
		"--steg",
		"LSB1",
	};
	TestAreOptionsValid(ConcatVectors(defaultArgs, std::vector<char*> {
		"--pass", 
		"hello world"
	}), true);
	TestAreOptionsValid(ConcatVectors(defaultArgs, std::vector<char*> {
	}), true);
	TestAreOptionsValid(ConcatVectors(defaultArgs, std::vector<char*> {
		"-a",
		"aes128"
	}), false);
	TestAreOptionsValid(ConcatVectors(defaultArgs, std::vector<char*> {
		"--pass",
		"hello wolrd"
		"-a",
		"aes256",
		"-m",
		"cfb"
	}), true);
}


void TestCipher(std::string message, std::string password, const EVP_CIPHER * cypher)
{
	Crypto::Encoder encoder;

	// encrypt
	auto cypherText = encoder.Cypher(std::stringstream(message), password, cypher, 1);
	// decrypt
	auto plainText = encoder.Cypher(std::stringstream(cypherText), password, cypher, 0);
	EXPECT_EQ(message, plainText);

}

TEST(StegoEncoder, TestingEncoding)	{
	TestCipher("hola mundo", "1234", EVP_aes_128_cbc());
	TestCipher("a", "contrasenia muy larga 12318319823u912838129381239128391283129831298319281123125134523452345345", EVP_aes_128_cfb());
	TestCipher("mensaje largoaskldfjas;lkdfj;laskjdf;lksajdf;lksajdf;lksajd;fkljaskldfj;lkasjdf;lk", "1", EVP_aes_256_ofb());
	TestCipher("hola mundo", "1234", EVP_des_cbc());
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}