#include <gtest/gtest.h>
#include <vector>
#include "../encryption.h"
#include "../encryption2.h"

TEST(EncryptAESTest, BasicTest) {
const std::vector<uint8_t> input = {0x01, 0x02, 0x03, 0x04, 0x05};

const std::vector<uint8_t> key = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};

const std::vector<uint8_t> expectedOutput = {/* ... */};

std::vector<uint8_t> output;

encryptAES(input, output, key);

ASSERT_EQ(output, expectedOutput);
}

TEST(EncryptionTest, RSAPublicEncryption) {
const uint8_t testData[] = "Hello, RSA!";
const size_t testDataSize = strlen(reinterpret_cast<const char*>(testData));

RSA* rsaKey = RSA_new();
BIGNUM* bn = BN_new();
BN_set_word(bn, RSA_F4);
RSA_generate_key_ex(rsaKey, 2048, bn, nullptr);

std::vector<uint8_t> encryptedData = encryptRSA(testData, testDataSize, rsaKey);

ASSERT_FALSE(encryptedData.empty());

RSA_free(rsaKey);
}