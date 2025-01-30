#include <jni.h>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>

extern "C" {
    JNIEXPORT void JNICALL Java_RSAExample_generateAndEncryptData(JNIEnv *env, jobject obj);
}

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

JNIEXPORT void JNICALL Java_RSAExample_generateAndEncryptData(JNIEnv *env, jobject obj) {
    // Initialize OpenSSL library
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // Generate RSA key pair using EVP_PKEY
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
    if (!ctx) {
        handleErrors();
    }

    // Set the key size (e.g., 2048 bits)
    if (EVP_PKEY_keygen_init(ctx) <= 0) {
        handleErrors();
    }

    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048) <= 0) {
        handleErrors();
    }

    // Generate the RSA key pair
    EVP_PKEY *pkey = nullptr;
    if (EVP_PKEY_keygen(ctx, &pkey) <= 0) {
        handleErrors();
    }
    EVP_PKEY_CTX_free(ctx);  // Free the context after key generation

    // Save private key to PEM (print to stdout)
    BIO *bio = BIO_new_fp(stdout, BIO_NOCLOSE);
    if (!PEM_write_bio_PrivateKey(bio, pkey, nullptr, nullptr, 0, nullptr, nullptr)) {
        handleErrors();
    }

    // Save public key to PEM (print to stdout)
    if (!PEM_write_bio_PUBKEY(bio, pkey)) {
        handleErrors();
    }

    std::cout << "\nPrivate and public keys written to stdout.\n";

    // Data to encrypt
    unsigned char data[] = "Hello OpenSSL 3.0!";
    unsigned char encrypted[256];
    unsigned char decrypted[256];

    // Encrypt data with the public key
    EVP_PKEY_CTX *enc_ctx = EVP_PKEY_CTX_new(pkey, nullptr);
    if (!enc_ctx) {
        handleErrors();
    }

    if (EVP_PKEY_encrypt_init(enc_ctx) <= 0) {
        handleErrors();
    }

    size_t encrypted_len;
    if (EVP_PKEY_encrypt(enc_ctx, encrypted, &encrypted_len, data, strlen((char*)data)) <= 0) {
        handleErrors();
    }

    EVP_PKEY_CTX_free(enc_ctx);  // Free encryption context

    std::cout << "\nEncrypted data (in hex): ";
    for (size_t i = 0; i < encrypted_len; ++i) {
        printf("%02x", encrypted[i]);
    }
    std::cout << "\n";

    // Decrypt the encrypted data with the private key
    EVP_PKEY_CTX *dec_ctx = EVP_PKEY_CTX_new(pkey, nullptr);
    if (!dec_ctx) {
        handleErrors();
    }

    if (EVP_PKEY_decrypt_init(dec_ctx) <= 0) {
        handleErrors();
    }

    size_t decrypted_len;
    if (EVP_PKEY_decrypt(dec_ctx, decrypted, &decrypted_len, encrypted, encrypted_len) <= 0) {
        handleErrors();
    }

    EVP_PKEY_CTX_free(dec_ctx);  // Free decryption context

    std::cout << "\nDecrypted data: " << decrypted << "\n";

    // Clean up
    EVP_PKEY_free(pkey);
    BIO_free(bio);
}
