//memory leak detecter #include <vld.h> 
#include <stdio.h>
#include <time.h>
#include "CoreOperation.h"
#include "RSA.h"
#define testMode 1 // Vaild answer checking = 0, Time checking = 1

int main()
{
	srand((unsigned)time(NULL));

//	bigint* a = NULL;
//	bigint* b = NULL;
//	bigint* c = NULL;
//	bigint* n = NULL;
//
//
//	int testCase = 5;
//	int bit = 1024;
//	int aWordlen = bit / WORD_UNIT; // 1024 - bit
//	int bWordlen = bit / WORD_UNIT; // 1024 - bit
//	int nWordlen = bit / WORD_UNIT; // 1024 - bit
//	int aSign = NON_NEGATIVE;
//	int bSign = NON_NEGATIVE;
//	int nSign = NON_NEGATIVE;
//	//aSign = rand() % 2;
//	//bSign = rand() % 2;
//	big_gen_rand(&a, aSign, aWordlen);
//	big_gen_rand(&b, bSign, bWordlen);
//	big_gen_rand(&n, nSign, nWordlen);
//
//	
//
//#if testMode == 1
//	for (int i = 0; i < 5; i++) {
//		clock_t start = clock();
//#endif
//		for (int t = 0; t < testCase; t++) {
//			/* generate random number */
//
//#if testMode == 0 
//			printf("a = ");
//			big_show_hex(a);
//			printf("b = ");
//			big_show_hex(b);
//			//printf("n = ");
//			//big_show_hex(n);
//#endif
//
//			/* operation */
//			big_mod_exp(&c, a, n, b);
//			//big_multiplication(&c, a, b);
//			//big_squaring(&c, a);
//
//#if testMode == 0
//			/* show */
//			//showProcessHex(a, b, c);
//
//			printf("c = ");
//			big_show_hex(c);
//			printf("if a * b == c :\n\t");
//			printf("print(\"True\")\n");
//			printf("else:\n");
//			printf("\tprint(\"False\")\n");
//#endif
//		}
//#if testMode == 1
//		clock_t end = clock();
//		float dif = (float)(end - start) / CLOCKS_PER_SEC;
//		dif /= testCase;
//		dif *= 10000;
//		printf("%.5f\n", dif);
//	}
//#endif
//	big_delete(&a);
//	big_delete(&b);
//	big_delete(&c);
//	big_delete(&n);

	bigint* publicKey = NULL;
	bigint* privateKey = NULL;
	bigint* plainText = NULL;
	bigint* cipherText = NULL;
	bigint* decCipherText = NULL;

	big_RSA_key_gen(&publicKey, &privateKey);
	printf("public key = ");
	big_show_hex(publicKey);
	printf("\n");
	printf("private key = ");
	big_show_hex(privateKey);
	printf("\n");
	big_gen_rand(&plainText, NON_NEGATIVE, 5);
	printf("message = ");
	big_show_hex(plainText);
	printf("\n");
	big_RSA_encryption(&cipherText, plainText, publicKey);
	printf("cipher = ");
	big_show_hex(cipherText);
	printf("\n");
	big_RSA_decryption(&decCipherText, cipherText, publicKey, privateKey);
	printf("decipher = ");
	big_show_hex(decCipherText);
	printf("\n");

	big_delete(&publicKey);
	big_delete(&privateKey);
	big_delete(&plainText);
	big_delete(&cipherText);
	big_delete(&decCipherText);

	return 0;
}