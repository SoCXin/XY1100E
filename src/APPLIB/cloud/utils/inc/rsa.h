#ifndef __RSA_H__
#define __RSA_H__

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    long long n;
    long long e;
} PUBLIC_KEY_Def;

typedef struct {
    long long n;
    long long d;
} PRIVATE_KEY_Def;

// This function will encrypt the data pointed to by message.
// The encrypted data will be 8 times as large as the original data.
extern void rsa_public_encrypt(const char *message, const unsigned long message_size, const PUBLIC_KEY_Def *pub, long long *encrypted);

extern void rsa_public_decrypt(const long *message, const unsigned long message_size, const PUBLIC_KEY_Def *pub, char *decrypted);

// This function will decrypt the data pointed to by message. 
// The variable message_size is the size in bytes of the encrypted message. 
// The decrypted data will be 1/8th the size of the encrypted data.
extern void rsa_private_decrypt(const long long *message, const unsigned long message_size, const PRIVATE_KEY_Def *priv, char *decrypted);

extern void rsa_private_encrypt(const char *message, const unsigned long message_size, const PRIVATE_KEY_Def *priv, long long *encrypted);

#endif
