#include "crypto.h"
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <string>
#include <vector>

std::string Crypto::base64Encode(const std::vector<unsigned char>& data) {
    BIO* bio = BIO_new(BIO_f_base64());
    BIO* mem = BIO_new(BIO_s_mem());
    bio = BIO_push(bio, mem);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);  // No newline
    BIO_write(bio, data.data(), data.size());
    BIO_flush(bio);

    char* encoded_data = nullptr;
    long length = BIO_get_mem_data(mem, &encoded_data);
    std::string encoded(encoded_data, length);
    BIO_free_all(bio);
    return encoded;
}

std::vector<unsigned char> Crypto::sha1Hash(const std::string& input) {
    std::vector<unsigned char> hash(SHA_DIGEST_LENGTH);
    SHA1(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(),
         hash.data());
    return hash;
}