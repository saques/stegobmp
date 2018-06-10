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
}

TEST(StegoEncoder, TestingEncoding)	{
	Crypto::Encoder encoder;
	auto options = SetUpArgcArgv(std::vector<char*> {"test", "--pass", "holamundo", "--in", "simpletext.txt"});
	auto cypher = encoder.Encode(options);
	auto options2 = SetUpArgcArgv(std::vector<char*> {"test", "--pass", "holamundo", "--in", "simpletextenc.txt"});
	auto plain = encoder.Decode(options2);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}