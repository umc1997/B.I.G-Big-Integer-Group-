#ifndef RSA_H
#define RSA_H
#include "CoreOperation.h"
#include "ErrorMessage.h"
#include "BigInt.h"
#include "config.h"

#if RSA_BIT == 1024
#define MR_TEST_ROUND 5
#elif RSA_BIT == 2048
#define MR_TEST_ROUND 5
#elif RSA_BIT == 3072
#define MR_TEST_ROUND 5
#elif RSA_BIT == 4096
#define MR_TEST_ROUND 5
#endif

ErrorMessage big_gen_secure_prime(bigint** p);

ErrorMessage big_RSA_key_gen(bigint** publicKey, bigint** privateKey);

ErrorMessage big_RSA_encipher(bigint** cipherText, bigint* plainText, bigint* publicKey);

ErrorMessage big_RSA_decipher(bigint** plainText, bigint* cipherText, bigint* publicKey, bigint* privateKey);

#endif