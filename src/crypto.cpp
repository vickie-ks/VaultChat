#include "crypto.h"
#include <vector>
#include <iostream>
#include <algorithm>

NTRUCrypto::NTRUCrypto(int N, int p, int q) : N(N), p(p), q(q) {}

std::vector<int> NTRUCrypto::polynomialMult(const std::vector<int>& a, const std::vector<int>& b, int mod) {
    std::vector<int> result(2 * N - 1, 0);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            result[i + j] += a[i] * b[j];
            result[i + j] %= mod;
        }
    }
    // Reduce the result to fit in polynomial of degree N
    std::vector<int> reducedResult(N, 0);
    for (int i = 0; i < N; ++i) {
        reducedResult[i] = result[i];
    }
    for (int i = N; i < 2 * N - 1; ++i) {
        reducedResult[i - N] += result[i];
        reducedResult[i - N] %= mod;
    }
    return reducedResult;
}

void NTRUCrypto::generateKeys() {
    f.clear(); h.clear();
    int f_values[] = {1, 0, 0, -1};
    int h_values[] = {2, 0, -1, 1};
    f.insert(f.end(), f_values, f_values + 4);
    h.insert(h.end(), h_values, h_values + 4);
}

std::vector<int> NTRUCrypto::encrypt(const std::vector<int>& message, const std::vector<int>& r) {
    std::vector<int> prh = polynomialMult(polynomialMult(r, h, q), std::vector<int>(N, p), q);
    std::vector<int> encryptedMessage(N);
    for (int i = 0; i < N; ++i) {
        encryptedMessage[i] = (prh[i] + message[i]) % q;
    }
    return encryptedMessage;
}

std::vector<int> NTRUCrypto::decrypt(const std::vector<int>& encryptedMessage) {
    std::vector<int> a = polynomialMult(f, encryptedMessage, q);
    std::vector<int> message(N);
    for (int i = 0; i < N; ++i) {
        message[i] = a[i] % p;
    }
    return message;
}
