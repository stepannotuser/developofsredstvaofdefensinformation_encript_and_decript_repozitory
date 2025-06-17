#include "Alghorutm.h"
#include <algorithm>
#include <functional>
#include <cstring>
#include <random>
#include <array>
#include <vector>
#include <iostream>

using namespace std; 

constexpr int SBOX_SIZE = 256;

class ChaCha20 {
public:
    ChaCha20(const array<uint32_t, 16>& key, uint32_t counter = 0) {
        state[0] = 0x61707865;
        state[1] = 0x3320646e;
        state[2] = 0x79622d36;
        state[3] = 0x6b206574;

        for (int i = 0; i < 8; ++i) {
            state[4 + i] = key[i];
        }
        state[12] = counter;
        state[13] = state[14] = state[15] = 0;
    }

    void encrypt(vector<unsigned char>& data) {
        vector<unsigned char> output(data.size());
        uint32_t tempState[16];
        uint32_t block[16];
        size_t byteIndex = 0;

        while (byteIndex < data.size()) {
            copy(begin(state), end(state), begin(tempState));

            for (int round = 0; round < 10; ++round) {
                quarterRound(tempState, 0, 4, 8, 12);
                quarterRound(tempState, 1, 5, 9, 13);
                quarterRound(tempState, 2, 6, 10, 14);
                quarterRound(tempState, 3, 7, 11, 15);

                quarterRound(tempState, 0, 5, 10, 15);
                quarterRound(tempState, 1, 6, 11, 12);
                quarterRound(tempState, 2, 7, 8, 13);
                quarterRound(tempState, 3, 4, 9, 14);
            }

            for (int i = 0; i < 16; ++i) {
                block[i] = tempState[i] + state[i];
            }

            for (size_t i = 0; i < 64 && byteIndex < data.size(); ++i, ++byteIndex) {
                output[byteIndex] = data[byteIndex] ^ (block[i / 4] >> (8 * (i % 4)) & 0xFF);
            }

            ++state[12]; 
        }

        data = move(output);
    }

private:
    uint32_t state[16];

    static void quarterRound(uint32_t* state, int a, int b, int c, int d) {
        state[a] += state[b];
        state[d] = (state[d] ^ state[a]) << 16 | (state[d] ^ state[a]) >> (32 - 16);
        state[c] += state[d];
        state[b] = (state[b] ^ state[c]) << 12 | (state[b] ^ state[c]) >> (32 - 12);
        state[a] += state[b];
        state[d] = (state[d] ^ state[a]) << 8 | (state[d] ^ state[a]) >> (32 - 8);
        state[c] += state[d];
        state[b] = (state[b] ^ state[c]) << 7 | (state[b] ^ state[c]) >> (32 - 7);
    }
};

Encryptor::Encryptor() {}

vector<unsigned char> Encryptor::generateKey(const string& password, size_t length) {
    vector<unsigned char> key(password.begin(), password.end());

    if (key.size() < length) {
        size_t original_size = key.size();
        key.resize(length);
        for (size_t i = original_size; i < length; ++i) {
            key[i] = key[i % original_size];
        }
    }

    if (key.size() > length) {
        key.resize(length);
    }

    return key;
}

Encryptor& Encryptor::getInstance() { // svoistvo patterna
    static Encryptor instance;
    return instance;
}

vector<unsigned char> Encryptor::encrypt(const vector<unsigned char>& data, const string& password) {
    if (data.empty()) return {};

    vector<unsigned char> key = generateKey(password, 32);
    array<uint32_t, 16> keyArray;
    for (int i = 0; i < 8; ++i) {
        keyArray[i] = (key[i * 4] << 24) | (key[i * 4 + 1] << 16) | (key[i * 4 + 2] << 8) | key[i * 4 + 3];
    }

    ChaCha20 cipher(keyArray);
    vector<unsigned char> result = data;
    cipher.encrypt(result);  

    return result;
}

vector<unsigned char> Encryptor::decrypt(const vector<unsigned char>& data, const string& password) {
    return encrypt(data, password);
}
