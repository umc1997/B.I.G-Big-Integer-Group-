#ifndef RSA_H
#define RSA_H
#include "CoreOperation.h"
#include "ErrorMessage.h"
#include "BigInt.h"
#include "config.h"

#define ENCRYPTION_KEY "10001"
#define PRIME_LIST_NUM 53

#if RSA_BIT == 1024
#define MR_TEST_ROUND 5
#elif RSA_BIT == 2048
#define MR_TEST_ROUND 5
#elif RSA_BIT == 3072
#define MR_TEST_ROUND 5
#elif RSA_BIT == 4096
#define MR_TEST_ROUND 5
#endif

ErrorMessage big_RSA_key_gen(bigint** publicKey, bigint** privateKey);

ErrorMessage big_RSA_encryption(bigint** cipherText, bigint* plainText, bigint* publicKey);

ErrorMessage big_RSA_decryption(bigint** plainText, bigint* cipherText, bigint* publicKey, bigint* privateKey);

#endif