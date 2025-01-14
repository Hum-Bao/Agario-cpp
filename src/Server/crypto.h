#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>
#include <vector>

class Crypto {
    public:
        static std::string base64Encode(const std::vector<unsigned char>& data);

        static std::vector<unsigned char> sha1Hash(const std::string& input);

    private:
};
#endif