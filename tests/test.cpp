#include <gtest/gtest.h>
#include <vector>
#include "../encryption.h"
#include "../encryption2.h"

TEST(EncryptAESTest, BasicTest) {
// Input data
const std::vector<uint8_t> input = {0x01, 0x02, 0x03, 0x04, 0x05};

// Key (replace with your actual key)
const std::vector<uint8_t> key = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};

// Expected output (replace with the expected result)
const std::vector<uint8_t> expectedOutput = {/* ... */};

// Output buffer
std::vector<uint8_t> output;

// Call the function
encryptAES(input, output, key);

// Compare the result with the expected output
ASSERT_EQ(output, expectedOutput);
}

TEST(EncryptionTest, RSAPublicEncryption) {
// Test data and RSA key
const uint8_t testData[] = "Hello, RSA!";
const size_t testDataSize = strlen(reinterpret_cast<const char*>(testData));

// Generate an RSA key (example key, replace with your actual key generation logic)
RSA* rsaKey = RSA_new();
BIGNUM* bn = BN_new();
BN_set_word(bn, RSA_F4);
RSA_generate_key_ex(rsaKey, 2048, bn, nullptr);

// Call the encryption function
std::vector<uint8_t> encryptedData = encryptRSA(testData, testDataSize, rsaKey);

// Check if the result is not empty
ASSERT_FALSE(encryptedData.empty());

// Clean up RSA key
RSA_free(rsaKey);
}