#include <iostream>
#include <openssl/md5.h>
#include <cstring>
int main() {

    const char * data = "hace mucho calor hoy";
    MD5_CTX origen;

    MD5_Init(&origen);
    auto md = static_cast<unsigned char *>(malloc (MD5_DIGEST_LENGTH));
    MD5_Update(&origen, data, strlen(data));
    MD5_Final(md, &origen);
    std::cout << md << std::endl;
    free (md);
    return EXIT_SUCCESS;
}