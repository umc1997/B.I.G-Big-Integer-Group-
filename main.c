//memory leak detecter #include <vld.h> 
#include <stdio.h>
#include <time.h>
#include "CoreOperation.h"
#include "RSA.h"
#include "test.h"

int main()
{
	srand((unsigned)time(NULL));

	printf("print(\"%d-bit Test\")\n", WORD_UNIT);

	additionTest();
	substractionTest();
	multiplicationTest();
	squaringTest();
	divisionTest();
	mod_expTest();

	return 0;
}






























//#if testMode == 1
//	for (int i = 0; i < 5; i++) {
//		clock_t start = clock();
//		clock_t end = clock();
//		float dif = (float)(end - start) / CLOCKS_PER_SEC;
//		dif /= testCase;
//		dif *= 10000;
//		printf("%.5f\n", dif);
//	}
//#endif


	//int testCase = 5;
	//int msglen = 1;
	//bigint* publicKey = NULL;
	//bigint* privateKey = NULL;
	//bigint* plainText = NULL;
	//bigint* cipherText = NULL;
	//bigint* decCipherText = NULL;

	//clock_t key_gen_start = clock();
	//big_RSA_key_gen(&publicKey, &privateKey);
	//clock_t key_gen_end = clock();
	//float key_gen_time = (float)(key_gen_end - key_gen_start) / CLOCKS_PER_SEC;
	//printf("Key generation time : %.5f\n", key_gen_time);

	//printf("public key = ");
	//big_show_hex(publicKey);
	//printf("\n");
	//printf("private key = ");
	//big_show_hex(privateKey);
	//printf("\n");

	//for (int i = 0; i < testCase; i++) {
	//	printf("Test %d\n\n", i + 1);
	//	// gen msg
	//	msglen = rand() % publicKey->wordlen;
	//	if (msglen == 0)
	//		msglen = 1;
	//	big_gen_rand(&plainText, NON_NEGATIVE, msglen);
	//	printf("message = ");
	//	big_show_hex(plainText);
	//	printf("\n");

	//	// encryption
	//	big_RSA_encipher(&cipherText, plainText, publicKey);
	//	printf("cipher = ");
	//	big_show_hex(cipherText);
	//	printf("\n");

	//	// decryption
	//	big_RSA_decipher(&decCipherText, cipherText, publicKey, privateKey);
	//	printf("decipher = ");
	//	big_show_hex(decCipherText);
	//	printf("\n");

	//	// check msg == decipher
	//	if (big_compare(plainText, decCipherText) == EQUAL)
	//		printf("message == decipher\n");
	//	printf("\n");

	//}

	//big_delete(&publicKey);
	//big_delete(&privateKey);
	//big_delete(&plainText);
	//big_delete(&cipherText);
	//big_delete(&decCipherText);

	//return 0;
