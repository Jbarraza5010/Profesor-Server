#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <vector>

void encryptAES(const std::vector<uint8_t>& input, std::vector<uint8_t>& output, const std::vector<uint8_t>& key);

#endif