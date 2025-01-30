#include <jni.h>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <cstring>

extern "C" {
    JNIEXPORT jobjectArray JNICALL Java_org_example_ExampleOpenSSL_generateKeyPair(JNIEnv *env, jobject obj, jint keyLength);
    JNIEXPORT jbyteArray JNICALL Java_org_example_ExampleOpenSSL_encryptData(JNIEnv *env, jobject obj, jstring publicKey, jbyteArray data);
    JNIEXPORT jbyteArray JNICALL Java_org_example_ExampleOpenSSL_decryptData(JNIEnv *env, jobject obj, jstring privateKey, jbyteArray encryptedData);
}

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

std::string privateKeyToString(EVP_PKEY *pkey) {
    BIO *bio = BIO_new(BIO_s_mem());
    if (!PEM_write_bio_PrivateKey(bio, pkey, nullptr, nullptr, 0, nullptr, nullptr)) {
        handleErrors();
    }

    char *data;
    long len = BIO_get_mem_data(bio, &data);
    std::string privateKey(data, len);
    BIO_free(bio);

    return privateKey;
}

std::string publicKeyToString(EVP_PKEY *pkey) {
    BIO *bio = BIO_new(BIO_s_mem());
    if (!PEM_write_bio_PUBKEY(bio, pkey)) {
        handleErrors();
    }

    char *data;
    long len = BIO_get_mem_data(bio, &data);
    std::string publicKey(data, len);
    BIO_free(bio);

    return publicKey;
}

JNIEXPORT jobjectArray JNICALL Java_org_example_ExampleOpenSSL_generateKeyPair(JNIEnv *env, jobject obj, jint keyLength) {
    // Initialize OpenSSL library
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // Validate the key length (only allow 512, 1024, 2048, or 4096 for security reasons)
    if (keyLength != 512 && keyLength != 1024 && keyLength != 2048 && keyLength != 4096) {
        keyLength = 2048;  // Set to default secure key size if invalid
        std::cerr << "Invalid key length. Using default key size: 2048 bits." << std::endl;
    }

    // Generate RSA key pair using EVP_PKEY
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
    if (!ctx) {
        handleErrors();
    }

    // Set the key size
    if (EVP_PKEY_keygen_init(ctx) <= 0) {
        handleErrors();
    }

    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, keyLength) <= 0) {
        handleErrors();
    }

    // Generate the RSA key pair
    EVP_PKEY *pkey = nullptr;
    if (EVP_PKEY_keygen(ctx, &pkey) <= 0) {
        handleErrors();
    }
    EVP_PKEY_CTX_free(ctx);

    // Convert the keys to strings
    std::string privateKey = privateKeyToString(pkey);
    std::string publicKey = publicKeyToString(pkey);

    // Prepare the return value
    jobjectArray result = env->NewObjectArray(2, env->FindClass("java/lang/String"), nullptr);
    env->SetObjectArrayElement(result, 0, env->NewStringUTF(privateKey.c_str()));
    env->SetObjectArrayElement(result, 1, env->NewStringUTF(publicKey.c_str()));

    EVP_PKEY_free(pkey);
    return result;
}

JNIEXPORT jbyteArray JNICALL Java_org_example_ExampleOpenSSL_encryptData(JNIEnv *env, jobject obj, jstring publicKey, jbyteArray data) {
    // Convert the public key string to EVP_PKEY
    const char *publicKeyStr = env->GetStringUTFChars(publicKey, nullptr);
    BIO *bio = BIO_new_mem_buf(publicKeyStr, -1);
    EVP_PKEY *pkey = PEM_read_bio_PUBKEY(bio, nullptr, nullptr, nullptr);
    BIO_free(bio);
    env->ReleaseStringUTFChars(publicKey, publicKeyStr);

    if (!pkey) {
        handleErrors();
    }

    // Initialize encryption context
    EVP_PKEY_CTX *enc_ctx = EVP_PKEY_CTX_new(pkey, nullptr);
    if (!enc_ctx) {
        handleErrors();
    }

    if (EVP_PKEY_encrypt_init(enc_ctx) <= 0) {
        handleErrors();
    }

    // Encrypt the data
    jsize dataLen = env->GetArrayLength(data);
    unsigned char *dataBytes = (unsigned char *)env->GetByteArrayElements(data, nullptr);

    size_t encrypted_len;
    unsigned char *encrypted = new unsigned char[EVP_PKEY_size(pkey)];
    if (EVP_PKEY_encrypt(enc_ctx, encrypted, &encrypted_len, dataBytes, dataLen) <= 0) {
        handleErrors();
    }

    EVP_PKEY_CTX_free(enc_ctx);

    // Create and return the encrypted data as a byte array
    jbyteArray encryptedData = env->NewByteArray(encrypted_len);
    env->SetByteArrayRegion(encryptedData, 0, encrypted_len, (jbyte *)encrypted);

    delete[] encrypted;
    EVP_PKEY_free(pkey);
    return encryptedData;
}

JNIEXPORT jbyteArray JNICALL Java_org_example_ExampleOpenSSL_decryptData(JNIEnv *env, jobject obj, jstring privateKey, jbyteArray encryptedData) {
    // Convert the private key string to EVP_PKEY
    const char *privateKeyStr = env->GetStringUTFChars(privateKey, nullptr);
    BIO *bio = BIO_new_mem_buf(privateKeyStr, -1);
    EVP_PKEY *pkey = PEM_read_bio_PrivateKey(bio, nullptr, nullptr, nullptr);
    BIO_free(bio);
    env->ReleaseStringUTFChars(privateKey, privateKeyStr);

    if (!pkey) {
        handleErrors();
    }

    // Initialize decryption context
    EVP_PKEY_CTX *dec_ctx = EVP_PKEY_CTX_new(pkey, nullptr);
    if (!dec_ctx) {
        handleErrors();
    }

    if (EVP_PKEY_decrypt_init(dec_ctx) <= 0) {
        handleErrors();
    }

    // Decrypt the data
    jsize encryptedLen = env->GetArrayLength(encryptedData);
    unsigned char *encryptedBytes = (unsigned char *)env->GetByteArrayElements(encryptedData, nullptr);

    size_t decrypted_len;
    unsigned char *decrypted = new unsigned char[EVP_PKEY_size(pkey)];
    if (EVP_PKEY_decrypt(dec_ctx, decrypted, &decrypted_len, encryptedBytes, encryptedLen) <= 0) {
        handleErrors();
    }

    EVP_PKEY_CTX_free(dec_ctx);

    // Create and return the decrypted data as a byte array
    jbyteArray decryptedData = env->NewByteArray(decrypted_len);
    env->SetByteArrayRegion(decryptedData, 0, decrypted_len, (jbyte *)decrypted);

    delete[] decrypted;
    EVP_PKEY_free(pkey);
    return decryptedData;
}
