package org.example;

public class ExampleOpenSSL {
    static {
        System.load(ExampleOpenSSL.class.getResource("/lib/ExampleOpenSSL.dll").getPath());
    }

    public native String[] generateKeyPair(int keyLength);
    public native byte[] encryptData(String publicKey, byte[] data);
    public native byte[] decryptData(String privateKey, byte[] encryptedData);

    public static void main(String[] args) {
        ExampleOpenSSL openssl = new ExampleOpenSSL();

        // Generate keys with 2048-bit length
        String[] keys = openssl.generateKeyPair(2048);
        String privateKey = keys[0];
        String publicKey = keys[1];

        System.out.println("Public Key:\n" + publicKey);
        System.out.println("Private Key:\n" + privateKey);

        // Encrypt and decrypt a message
        String message = "Hello, variable RSA key length!";
        byte[] encrypted = openssl.encryptData(publicKey, message.getBytes());
        System.out.println("Encrypted: " + new String(encrypted));

        byte[] decrypted = openssl.decryptData(privateKey, encrypted);
        System.out.println("Decrypted: " + new String(decrypted));
    }
}