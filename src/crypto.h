#ifndef CRYPTO_H
#define CRYPTO_H

#include <vector>

class NTRUCrypto {
public:
    NTRUCrypto(int N, int p, int q);
    void generateKeys();
    std::vector<int> encrypt(const std::vector<int>& message, const std::vector<int>& r);
    std::vector<int> decrypt(const std::vector<int>& encryptedMessage);

private:
    int N;  // Degree of the polynomial
    int p;  // Modulus for message coefficients
    int q;  // Modulus for polynomial coefficients
    std::vector<int> f;  // Private key
    std::vector<int> fp; // Private key inverse modulo p
    std::vector<int> fq; // Private key inverse modulo q
    std::vector<int> h;  // Public key

    std::vector<int> modInverse(const std::vector<int>& a, int mod);
    std::vector<int> polynomialMult(const std::vector<int>& a, const std::vector<int>& b, int mod);
};

#endif // CRYPTO_H
