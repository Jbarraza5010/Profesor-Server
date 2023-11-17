#ifndef ENCRYPTION2_H
#define ENCRYPTION2_H

#include <vector>
#include <openssl/rsa.h>

std::vector<uint8_t> encryptRSA(const uint8_t* data, size_t dataSize, RSA* rsaKey);

#endif

