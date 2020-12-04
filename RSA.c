#include "RSA.h"

#define ENCRYPTION_KEY "10001"
#define PRIME_LIST_NUM 53

static ErrorMessage big_gen_secure_prime(bigint** p);
static bool big_isPrime(bigint* p);
static bool big_isSecurePrime(bigint* p);
static ErrorMessage big_gen_nbit_prime(bigint** p);

/**
 * Generate public key and private key for RSA.
 * 
 * \param publicKey : address of public key for encipher and decipher
 * \param privateKey : addres of private key for decipher
 * \return : ErrorMessage
 */
ErrorMessage big_RSA_key_gen(bigint** publicKey, bigint** privateKey)
{
	bigint* p = NULL;
	bigint* q = NULL;
	bigint* N = NULL;
	bigint* e = NULL;
	bigint* d = NULL;
	bigint* phi = NULL;
	bigint* minusOne = NULL;
	bigint* check = NULL;

	big_set_zero(&check);
	do {
		// 1. generate two n/2-bit prime p,q
		big_gen_secure_prime(&p);
		big_gen_secure_prime(&q);
		// p != q
		if (big_compare(p, q) == EQUAL)
			continue;

		// 2. calculate N = p * q and phi = (p - 1) * (q - 1)
		big_multiplication(&N, p, q);
		big_set_by_string(&minusOne, NEGATIVE, "1", 16);
		big_addition(&p, p, minusOne);
		big_addition(&q, q, minusOne);
		big_multiplication(&phi, p, q);

		// 3. e = 0x10001
		big_set_by_string(&e, NON_NEGATIVE, ENCRYPTION_KEY, 16);
		/* gcd(e,phi) must be 1 */
		big_gcd(&check, e, phi);

	} while (!big_is_one(check));

	// 4. calculate d, such that ed = 1(mod phi(N))
	big_mod_inverse(&d, e, phi);

	// 5. publicKey <- N , privateKey <- d
	big_assign(publicKey, N);
	big_assign(privateKey, d);

	big_delete(&p);
	big_delete(&q);
	big_delete(&N);
	big_delete(&e);
	big_delete(&d);
	big_delete(&phi);
	big_delete(&minusOne);
	big_delete(&check);

	return SUCCESS;
}

/**
 * Encipher a plain text to cipher text using public key.
 * 
 * \param cipherText : address of cipher text
 * \param plainText : plain text to encipher
 * \param publicKey : public key
 * \return : ErrorMessage
 */
ErrorMessage big_RSA_encipher(bigint** cipherText, bigint* plainText, bigint* publicKey)
{
	if (plainText == NULL || publicKey == NULL) return FAIL_NULL;
	if (big_compare(publicKey, plainText) == SMALLER) return FAIL_INVALID_INPUT; // plain text must be smaller than public key
	bigint* e = NULL;
	bigint* tmp = NULL;

	big_set_by_string(&e, NON_NEGATIVE, ENCRYPTION_KEY, 16);
	big_mod_exp(&tmp, plainText, e, publicKey);
	big_assign(cipherText, tmp);

	big_delete(&e);
	big_delete(&tmp);
	return SUCCESS;
}

/**
 * Decipher a cipher tect to plain text using public key and private key.
 * 
 * \param plainText : address of plain text
 * \param cipherText : cipher text to decipher
 * \param publicKey : public key
 * \param privateKey : private key
 * \return :ErrorMessage
 */
ErrorMessage big_RSA_decipher(bigint** plainText, bigint* cipherText, bigint* publicKey, bigint* privateKey)
{
	if (cipherText == NULL || privateKey == NULL)
		return FAIL_NULL;

	bigint* tmp = NULL;

	big_mod_exp(&tmp, cipherText, privateKey, publicKey);
	big_assign(plainText, tmp);

	big_delete(&tmp);
	return SUCCESS;
}

//static function
static ErrorMessage big_gen_secure_prime(bigint** p)
{
	bigint* tmp = NULL;
	do
	{
		big_gen_nbit_prime(&tmp);
	} while (big_isSecurePrime(tmp));
	//check (p-1)/2 is prime?

	big_assign(p, tmp);
	big_delete(&tmp);
	return SUCCESS;
}
//static function
static ErrorMessage big_gen_nbit_prime(bigint** p)
{
	bigint* tmp = NULL;
	bigint* two = NULL;
	big_set_by_string(&two, NON_NEGATIVE, "2", 16);
#if WORD_UNIT == 8
	int wordlen = RSA_BIT >> 4;
#elif WORD_UNIT == 32
	int wordlen = RSA_BIT >> 6;
#elif WORD_UNIT == 64
	int wordlen = RSA_BIT >> 7;
#endif

	// gen random bigint
	big_gen_rand(&tmp, NON_NEGATIVE, wordlen);

	// set MSB 1 
	word MSW = tmp->a[wordlen - 1];
	tmp->a[wordlen - 1] = MSW | ((word)1 << (WORD_UNIT - 1));

	// set odd
	if (big_is_even(tmp))
	{
		bigint* one = NULL;
		big_set_one(&one);
		big_addition(&tmp, tmp, one);
		big_delete(&one);
	}

	// loop while find prime
	while (!big_isPrime(tmp))
	{
		big_addition(&tmp, tmp, two);
	}

	big_assign(p, tmp);

	big_delete(&tmp);
	big_delete(&two);
	return SUCCESS;
}
//static function
static bool big_isPrime(bigint* p)
{
	int PRIME_LIST[PRIME_LIST_NUM] = {
	3, 5, 7, 11, 13, 17, 19, 23, 29, 31,
	37, 41, 43, 47, 53, 59, 61, 67, 71,
	73, 79, 83, 89, 97, 101, 103, 107,
	109, 113, 127, 131, 137, 139, 149,
	151, 157, 163, 167, 173, 179, 181,
	191, 193, 197, 199, 211, 223, 227,
	229, 233, 239, 241, 251
	};

	// low level test
	bigint* smallPrime = NULL;
	bigint* remainder = NULL;
	bool isPrime = true;
	for (int i = 0; i < PRIME_LIST_NUM; i++)
	{
		word smallPrimeWord[1] = { PRIME_LIST[i] };
		big_set_by_array(&smallPrime, NON_NEGATIVE, smallPrimeWord, 1);
		big_mod(&remainder, p, smallPrime);
		if (big_is_zero(remainder))
		{
			isPrime = false;
			break;
		}
	}
	big_delete(&smallPrime);
	big_delete(&remainder);
	
	if (!isPrime)
		return isPrime;

	//miller-rabin test
	bigint* one = NULL;
	big_set_one(&one);

	// find q and l such that p - 1 = 2 ^ l * q 
	int maxDivisionByTwo = 0; // l
	bigint* divisionByTwo = NULL;// q
	bigint* Pminus1 = NULL;
	big_substraction(&Pminus1, p, one);
	big_assign(&divisionByTwo, Pminus1);
	while (big_is_even(divisionByTwo))
	{
		big_bit_right_shift(&divisionByTwo, divisionByTwo, 1);
		maxDivisionByTwo += 1;
	}
	// gen a such that 2 < a < p
	bigint* a = NULL;
	bool probablyPrime = false;
	for (int i = 0; i < MR_TEST_ROUND; i++)
	{
		do
		{
			big_gen_rand(&a, NON_NEGATIVE, 2);
		} while (big_is_one(a) || big_is_zero(a));

		// checking gcd(a, p) = 1
		if (!big_is_relatively_prime(a, p))
		{
			isPrime = false;
			break;
		}

		// a = a ^ q mod p
		big_mod_exp(&a, a, divisionByTwo, p);
		if (big_is_one(a))
		{
			probablyPrime = true;
			continue;
		}

		// a = a ^ 2q mod p 
		bigint* tmp = NULL; 
		for (int i = 0; i < maxDivisionByTwo; i++)
		{
			// tmp = a + 1
			big_addition(&tmp, a, one);
			// tmp = tmp mod p 
			big_mod(&tmp, tmp, p);
			// tmp = 0 => a = -1 (mod p) => p is composite
			if (big_is_zero(tmp)) {
				probablyPrime = true;
				break; 
			}
			big_squaring(&a, a);
			big_mod(&a, a, p);
		}
		big_delete(&tmp);
		if (probablyPrime)
			continue;
		isPrime = false;
	}

	big_delete(&one);
	big_delete(&divisionByTwo);
	big_delete(&Pminus1);
	big_delete(&a);

	return isPrime;
}
//static function
static bool big_isSecurePrime(bigint* p)
{
	bigint* tmp = NULL;
	big_bit_right_shift(&tmp, p, 1);
	bool ret = big_isPrime(tmp);
	big_delete(&tmp);
	return ret;
}