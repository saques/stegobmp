#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <openssl/md5.h>
#include <openssl/evp.h>
#include <cxxopts.hpp>
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

int main() 
{
    cxxopts::Options options("My program", "hello");
    bool compile = false;

    LOG(INFO) << "HELLO WORLD!!!";
    options.add_options()
    ("f,file", "File", cxxopts::value<std::string>())
    ("c,compile", "Compile", cxxopts::value(compile));

    const char * data = "hace mucho calor hoy";
    MD5_CTX origen;

    MD5_Init(&origen);
    auto md = static_cast<unsigned char *>(malloc (MD5_DIGEST_LENGTH));
    MD5_Update(&origen, data, strlen(data));
    MD5_Final(md, &origen);
    std::cout << md << std::endl;
    free (md);
    std::cout << "hello world" << std::endl;
    return EXIT_SUCCESS;
}