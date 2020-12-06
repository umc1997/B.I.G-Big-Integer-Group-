#include "test.h"

void showProcess(bigint* c, bigint* a, bigint* b, char op) {
	printf("a = ");
	big_show_hex(a);
	printf("b = ");
	big_show_hex(b);
	printf("c = ");
	big_show_hex(c);
	printf("if a %c b != c :\n", op);
	printf("\tflag = 0 \n");
	printf("\tprint(hex(c)) \n");
}
void showProcessDiv(bigint* q, bigint* r, bigint* a, bigint* b) {
	printf("a = ");
	big_show_hex(a);
	printf("b = ");
	big_show_hex(b);
	printf("q = ");
	big_show_hex(q);
	printf("r = ");
	big_show_hex(r);
	printf("if a != b * q + r or r >= b :\n");
	printf("\tflag = 0 \n");
	printf("\tprint(hex(q)) \n");
	printf("\tprint(hex(r)) \n");
}
void showProcessModExp(bigint* c, bigint* a, bigint* n, bigint* b)
{
	printf("a = ");
	big_show_hex(a);
	printf("b = ");
	big_show_hex(b);
	printf("n = ");
	big_show_hex(n);
	printf("c = ");
	big_show_hex(c);
	printf("if c != pow(a, n, b) :\n");
	printf("\tflag = 0 \n");
	printf("\tprint(hex(pow(a, n, b))) \n");
	printf("\tprint(hex(c)) \n");
}
void show_hex_without_prefix(bigint* x)
{
	int wordlen = x->wordlen;
	for (int i = wordlen - 1; i >= 0; i--)
	{
#if WORD_UNIT==8
		printf("%02x", x->a[i]);
#elif WORD_UNIT==32
		printf("%08x", x->a[i]);
#elif WORD_UNIT==64
		printf("%016llx", x->a[i]);
#endif
	}
	printf("\n");
}

void additionTest()
{
	bigint* a = NULL;
	bigint* b = NULL;
	bigint* c = NULL;
	int aSignT[4] = { NON_NEGATIVE,NON_NEGATIVE, NEGATIVE, NEGATIVE, };
	int bSignT[4] = { NON_NEGATIVE,NEGATIVE, NON_NEGATIVE, NEGATIVE, };
	int wordlen = testBitlen / WORD_UNIT;

	printf("print(\"Addition Test\")\n");
	printf("flag = 1\n");
	// c = a + b
	// test 1 : a = 0
	big_set_zero(&a);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	big_addition(&c, a, b);
	showProcess(c, a, b, '+');

	// test 2 : sign test
	//			(a > 0 & b > 0) (a > 0 & b < 0) (a < 0 & b > 0) (a < 0 & b < 0) 
	for (int i = 0; i < 4; i++)
	{
		big_gen_rand(&a, aSignT[i], wordlen);
		big_gen_rand(&b, bSignT[i], wordlen);
		big_addition(&c, a, b);
		showProcess(c, a, b, '+');
	}

	// test 3 : random bigint test
	for (int i = 0; i < testCase; i++)
	{
		int aSign = rand() % 2;
		int bSign = rand() % 2;
		int aWordlen = rand() % wordlen + 1;
		int bWordlen = rand() % wordlen + 1;
		big_gen_rand(&a, aSign, aWordlen);
		big_gen_rand(&b, bSign, bWordlen);
		big_addition(&c, a, b);
		showProcess(c, a, b, '+');
	}
	big_delete(&a);
	big_delete(&b);
	big_delete(&c);
	printf("if flag :\n");
	printf("\tprint(\"Addition Test Passed\")\n");
	printf("else:\n");
	printf("\tprint(\"Addition Test Failed\")\n");
}
void subtractionTest()
{
	bigint* a = NULL;
	bigint* b = NULL;
	bigint* c = NULL;
	int wordlen = testBitlen / WORD_UNIT;

	printf("print(\"Subtraction Test\")\n");
	printf("flag = 1\n");
	// c = a - b
	// test 1 : a = 0
	big_set_zero(&a);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	big_subtraction(&c, a, b);
	showProcess(c, a, b, '-');

	// test 2 : b = 0
	big_gen_rand(&a, NON_NEGATIVE, wordlen);
	big_set_zero(&b);
	big_subtraction(&c, a, b);
	showProcess(c, a, b, '-');

	// test 3 : a = b
	big_gen_rand(&a, NON_NEGATIVE, wordlen);
	big_assign(&b, a);
	big_subtraction(&c, a, b);
	showProcess(c, a, b, '-');

	// test 4 : sign test
	//			(a > b > 0)  (b > a > 0)
	big_gen_rand(&a, NON_NEGATIVE, wordlen);
	big_gen_rand(&b, NON_NEGATIVE, wordlen - 1);
	big_subtraction(&c, a, b);
	showProcess(c, a, b, '-');

	big_gen_rand(&a, NON_NEGATIVE, wordlen - 1);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	big_subtraction(&c, a, b);
	showProcess(c, a, b, '-');

	//			(0 > a > b) (0 > b > a)
	big_gen_rand(&a, NEGATIVE, wordlen);
	big_gen_rand(&b, NEGATIVE, wordlen - 1);
	big_subtraction(&c, a, b);
	showProcess(c, a, b, '-');

	big_gen_rand(&a, NEGATIVE, wordlen - 1);
	big_gen_rand(&b, NEGATIVE, wordlen);
	big_subtraction(&c, a, b);
	showProcess(c, a, b, '-');

	//			(a > 0 & b < 0) (a < 0 & b > 0)   
	big_gen_rand(&a, NON_NEGATIVE, wordlen);
	big_gen_rand(&b, NEGATIVE, wordlen);
	big_subtraction(&c, a, b);
	showProcess(c, a, b, '-');

	big_gen_rand(&a, NEGATIVE, wordlen);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	big_subtraction(&c, a, b);
	showProcess(c, a, b, '-');

	// test 5 : random bigint test
	for (int i = 0; i < testCase; i++)
	{
		int aSign = rand() % 2;
		int bSign = rand() % 2;
		int aWordlen = rand() % wordlen + 1;
		int bWordlen = rand() % wordlen + 1;
		big_gen_rand(&a, aSign, aWordlen);
		big_gen_rand(&b, bSign, bWordlen);
		big_subtraction(&c, a, b);
		showProcess(c, a, b, '-');
	}

	big_delete(&a);
	big_delete(&b);
	big_delete(&c);
	printf("if flag :\n");
	printf("\tprint(\"Subtraction Test Passed\")\n");
	printf("else:\n");
	printf("\tprint(\"Subtraction Test Failed\")\n");
}
void multiplicationTest()
{
	bigint* a = NULL;
	bigint* b = NULL;
	bigint* c = NULL;
	int aSignT[4] = { NON_NEGATIVE,NON_NEGATIVE, NEGATIVE, NEGATIVE, };
	int bSignT[4] = { NON_NEGATIVE,NEGATIVE, NON_NEGATIVE, NEGATIVE, };
	int wordlen = testBitlen / WORD_UNIT;

	printf("print(\"Multiplication Test\")\n");
	printf("flag = 1\n");
	// c = a * b
	// test 1 : a = 0
	big_set_zero(&a);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	big_multiplication(&c, a, b);
	showProcess(c, a, b, '*');

	// test 2 : a = 1
	big_set_one(&a);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	big_multiplication(&c, a, b);
	showProcess(c, a, b, '*');

	// test 3 : a = -1
	big_set_one(&a);
	big_flip_sign(&a);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	big_multiplication(&c, a, b);
	showProcess(c, a, b, '*');

	// test 4 : a = b -> squaring
	big_gen_rand(&a, NON_NEGATIVE, wordlen);
	big_assign(&b, a);
	big_multiplication(&c, a, b);
	showProcess(c, a, b, '*');

	// test 5 : sign test
	//			(a > 0 & b > 0) (a > 0 & b < 0) (a < 0 & b > 0) (a < 0 & b < 0) 
	for (int i = 0; i < 4; i++)
	{
		big_gen_rand(&a, aSignT[i], wordlen);
		big_gen_rand(&b, bSignT[i], wordlen);
		big_multiplication(&c, a, b);
		showProcess(c, a, b, '*');
	}

	// test 6 : random bigint test
	for (int i = 0; i < testCase; i++)
	{
		int aSign = rand() % 2;
		int bSign = rand() % 2;
		int aWordlen = rand() % wordlen + 1;
		int bWordlen = rand() % wordlen + 1;
		big_gen_rand(&a, aSign, aWordlen);
		big_gen_rand(&b, bSign, bWordlen);
		big_multiplication(&c, a, b);
		showProcess(c, a, b, '*');
	}

	big_delete(&a);
	big_delete(&b);
	big_delete(&c);
	printf("if flag :\n");
	printf("\tprint(\"Multiplication Test Passed\")\n");
	printf("else:\n");
	printf("\tprint(\"Multiplication Test Failed\")\n");
}
void squaringTest()
{
	bigint* a = NULL;
	bigint* c = NULL;
	int wordlen = testBitlen / WORD_UNIT;

	printf("print(\"Squaring Test\")\n");
	printf("flag = 1\n");
	// c = a * a
	// test 1 : a = 0
	big_set_zero(&a);
	big_squaring(&c, a);
	showProcess(c, a, a, '*');

	// test 2 : a = 1
	big_set_one(&a);
	big_squaring(&c, a);
	showProcess(c, a, a, '*');

	// test 3 : a = -1
	big_set_one(&a);
	big_flip_sign(&a);
	big_squaring(&c, a);
	showProcess(c, a, a, '*');

	// test 4 : sign test
	//			(a > 0) (a < 0) 
	big_gen_rand(&a, NON_NEGATIVE, wordlen);
	big_squaring(&c, a);
	showProcess(c, a, a, '*');

	big_gen_rand(&a, NEGATIVE, wordlen);
	big_squaring(&c, a);
	showProcess(c, a, a, '*');


	// test 5 : random bigint test
	for (int i = 0; i < testCase; i++)
	{
		int aSign = rand() % 2;
		int aWordlen = rand() % wordlen + 1;
		big_gen_rand(&a, aSign, aWordlen);
		big_squaring(&c, a);
		showProcess(c, a, a, '*');
	}

	big_delete(&a);
	big_delete(&c);
	printf("if flag :\n");
	printf("\tprint(\"Squaring Test Passed\")\n");
	printf("else:\n");
	printf("\tprint(\"Squaring Test Failed\")\n");

}
void divisionTest()
{
	bigint* a = NULL;
	bigint* b = NULL;
	bigint* q = NULL;
	bigint* r = NULL;
	int wordlen = testBitlen / WORD_UNIT;

	printf("print(\"Division Test\")\n");
	printf("flag = 1\n");
	// a = b * q + r (0 < r <= b)
	// test 1 : a = b
	big_gen_rand(&a, NON_NEGATIVE, wordlen);
	big_assign(&b, a);
	big_division(&q, &r, a, b);
	showProcessDiv(q, r, a, b);

	// test 2 : a < b
	big_gen_rand(&a, NON_NEGATIVE, wordlen);
	big_gen_rand(&b, NON_NEGATIVE, wordlen + 1);
	big_division(&q, &r, a, b);
	showProcessDiv(q, r, a, b);

	// test 3 : a > 0
	big_gen_rand(&a, NON_NEGATIVE, wordlen * 2);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	big_division(&q, &r, a, b);
	showProcessDiv(q, r, a, b);

	// test 4 : a < 0
	big_gen_rand(&a, NEGATIVE, wordlen * 2);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	big_division(&q, &r, a, b);
	showProcessDiv(q, r, a, b);

	// test 5 : random bigint test
	for (int i = 0; i < testCase; i++)
	{
		int aSign = rand() % 2;
		int aWordlen = rand() % wordlen + 1;
		int bWordlen = rand() % wordlen + 1;
		big_gen_rand(&a, aSign, aWordlen);
		big_gen_rand(&b, NON_NEGATIVE, bWordlen);
		big_division(&q, &r, a, b);
		showProcessDiv(q, r, a, b);
	}

	big_delete(&a);
	big_delete(&b);
	big_delete(&q);
	big_delete(&r);
	printf("if flag :\n");
	printf("\tprint(\"Division Test Passed\")\n");
	printf("else:\n");
	printf("\tprint(\"Division Test Failed\")\n");
}
void mod_expTest()
{
	bigint* a = NULL;
	bigint* b = NULL;
	bigint* n = NULL;
	bigint* c = NULL;
	int wordlen = testBitlen / WORD_UNIT;

	printf("print(\"Modular Exponentiation Test\")\n");
	printf("flag = 1\n");
	// c = a ^ n mod b
	// test 1 : a = 0
	big_set_zero(&a);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	big_gen_rand(&n, NON_NEGATIVE, wordlen);
	big_mod_exp(&c, a, n, b);
	showProcessModExp(c, a, n, b);

	// test 2 : a > 0
	big_gen_rand(&a, NON_NEGATIVE, wordlen);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	big_gen_rand(&n, NON_NEGATIVE, wordlen);
	big_mod_exp(&c, a, n, b);
	showProcessModExp(c, a, n, b);

	// test 3 : a < 0
	big_gen_rand(&a, NEGATIVE, wordlen);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	big_gen_rand(&n, NON_NEGATIVE, wordlen);
	big_mod_exp(&c, a, n, b);
	showProcessModExp(c, a, n, b);

	// test 4 : random bigint test
	for (int i = 0; i < 10; i++)
	{
		int aSign = rand() % 2;
		int aWordlen = rand() % wordlen + 1;
		int bWordlen = rand() % wordlen + 1;
		int nWordlen = rand() % wordlen + 1;
		big_gen_rand(&a, aSign, aWordlen);
		big_gen_rand(&b, NON_NEGATIVE, bWordlen);
		big_gen_rand(&n, NON_NEGATIVE, nWordlen);
		big_mod_exp(&c, a, n, b);
		showProcessModExp(c, a, n, b);
	}

	big_delete(&a);
	big_delete(&b);
	big_delete(&n);
	big_delete(&c);
	printf("if flag :\n");
	printf("\tprint(\"Modular Exponentiation Test Passed\")\n");
	printf("else:\n");
	printf("\tprint(\"Modular Exponentiation Test Failed\")\n");
}
void RSATest()
{
	int msglen = 1;
	bigint* publicKey = NULL;
	bigint* privateKey = NULL;
	bigint* plainText = NULL;
	bigint* cipherText = NULL;
	bigint* decCipherText = NULL;

	//clock_t key_gen_start = clock();
	//big_RSA_key_gen(&publicKey, &privateKey);
	//clock_t key_gen_end = clock();
	//float key_gen_time = (float)(key_gen_end - key_gen_start) / CLOCKS_PER_SEC;
	//printf("Key generation time : %.5f\n", key_gen_time);
	int command = 0;
	while (1)
	{
		printf("0. Exit\n");
		printf("1. Key Generate\n");
		printf("2. Encipher\n");
		printf("3. Decipher\n");
		printf("Enter the command : ");
		int a = scanf("%d", &command);
		if (command == 0) break;
		else if (command == 1)
		{
			big_RSA_key_gen(&publicKey, &privateKey);
			printf("public key : ");
			show_hex_without_prefix(publicKey);
			printf("\n");
			printf("private key : ");
			show_hex_without_prefix(privateKey);
			printf("\n");
		}
		else if (command == 2)
		{
			char msg[RSA_BIT / sizeof(char)];
			char pubKey[RSA_BIT / sizeof(char)];

			printf("Enter message : ");
			a = scanf("%s", msg);
			big_set_by_string(&plainText, NON_NEGATIVE, msg, 16);
			printf("\n");

			printf("Enter public key (Don't write prefix) : ");
			a = scanf("%s", pubKey);
			big_set_by_string(&publicKey, NON_NEGATIVE, pubKey, 16);
			big_RSA_encipher(&cipherText, plainText, publicKey);
			printf("\n");

			printf("Cipher text : ");
			show_hex_without_prefix(cipherText);
		}
		else if (command == 3)
		{
			char msg[RSA_BIT / sizeof(char)];
			char pubKey[RSA_BIT / sizeof(char)];
			char priKey[RSA_BIT / sizeof(char)];
			printf("Enter message : ");
			a = scanf("%s", msg);
			printf("\n");
			printf("Enter public key (Don't write prefix) : ");
			a = scanf("%s", pubKey);
			big_set_by_string(&publicKey, NON_NEGATIVE, pubKey, 16);
			printf("\n");
			printf("Enter private key (Don't write prefix) : ");
			a = scanf("%s", priKey);
			big_set_by_string(&privateKey, NON_NEGATIVE, priKey, 16);

			big_RSA_decipher(&decCipherText, cipherText, publicKey, privateKey);
			printf("\n");
			printf("Original message : ");
			show_hex_without_prefix(decCipherText);
		}
		printf("\n");
	}
	big_delete(&publicKey);
	big_delete(&privateKey);
	big_delete(&plainText);
	big_delete(&cipherText);
	big_delete(&decCipherText);
}
void decimalTest()
{
	char decimal[1000];
	bigint* a = NULL;
	printf("Enter decimal number (Don't write prefix) : ");
	int tmp = scanf("%s", decimal);
	big_set_by_string(&a, NON_NEGATIVE, decimal, 10);

	printf("hex : ");
	big_show_hex(a);

	printf("dec : ");
	big_show_dec(a);

	big_delete(&a);
}
void gcdTest()
{
	bigint* a = NULL;
	bigint* b = NULL;
	bigint* c = NULL;
	int wordlen = testBitlen / WORD_UNIT;

	printf("print(\"GCD Test\")\n");
	printf("from math import gcd\n");
	printf("flag = 1\n");
	for (int i = 0; i < testCase; i++)
	{
		int aWordlen = rand() % wordlen + 1;
		int bWordlen = rand() % wordlen + 1;
		big_gen_rand(&a, NON_NEGATIVE, aWordlen);
		big_gen_rand(&b, NON_NEGATIVE, bWordlen);
		big_gcd(&c, a, b);
		printf("a = ");
		big_show_hex(a);
		printf("b = ");
		big_show_hex(b);
		printf("c = ");
		big_show_hex(c);
		printf("if gcd(a, b) != c :\n");
		printf("\tflag = 0 \n");
		printf("\tprint(hex(c)) \n");
	}
	printf("if flag :\n");
	printf("\tprint(\"GCD Test Passed\")\n");
	printf("else:\n");
	printf("\tprint(\"GCD Test Failed\")\n");

	big_delete(&a);
	big_delete(&b);
	big_delete(&c);
}
void modular_inverseTest()
{
	bigint* a = NULL;
	bigint* b = NULL;
	bigint* c = NULL;
	int wordlen = testBitlen / WORD_UNIT;
	printf("print(\"Inverse Test\")\n");
	printf("flag = 1\n");

	big_set_by_string(&b, NON_NEGATIVE, "97", 10);
	for (int i = 0; i < testCase; i++)
	{
		int aWordlen = 1;
		do {
			big_gen_rand(&a, NON_NEGATIVE, aWordlen);
		} while (!big_is_relatively_prime(a, b));
		big_mod_inverse(&c, a, b);
		printf("a = ");
		big_show_hex(a);
		printf("b = ");
		big_show_hex(b);
		printf("c = ");
		big_show_hex(c);
		printf("if a * c %% b != 1 :\n");
		printf("\tflag = 0 \n");
		printf("\tprint(hex(a)) \n");
		printf("\tprint(hex(c)) \n");
	}

	printf("if flag :\n");
	printf("\tprint(\"Inverse Test Passed\")\n");
	printf("else:\n");
	printf("\tprint(\"Inverse Test Failed\")\n");

	big_delete(&a);
	big_delete(&b);
	big_delete(&c);
}

void additionTimeTest()
{
	bigint* a = NULL;
	bigint* b = NULL;
	bigint* c = NULL;
	int wordlen = testBitlen / WORD_UNIT;
	int t = testCase * 10;
	big_gen_rand(&a, NON_NEGATIVE, wordlen);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	clock_t start = clock();
	for (int i = 0; i < t; i++)
	{
		big_addition(&c, a, b);
	}
	clock_t end = clock();
	float dif = (float)(end - start)/ CLOCKS_PER_SEC;
	dif /= t;
	dif *= 10000;
	printf("%.5lf\n", dif);

	big_delete(&a);
	big_delete(&b);
	big_delete(&c);
}
void subtractionTimeTest()
{
	bigint* a = NULL;
	bigint* b = NULL;
	bigint* c = NULL;
	int wordlen = testBitlen / WORD_UNIT;
	int t = testCase * 10;
	big_gen_rand(&a, NON_NEGATIVE, wordlen);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	clock_t start = clock();
	for (int i = 0; i < t; i++)
	{
		big_subtraction(&c, a, b);
	}
	clock_t end = clock();
	float dif = (float)(end - start) / CLOCKS_PER_SEC;
	dif /= t;
	dif *= 10000;
	printf("%.5f\n", dif);

	big_delete(&a);
	big_delete(&b);
	big_delete(&c);
}
void multiplicationTimeTest()
{
	bigint* a = NULL;
	bigint* b = NULL;
	bigint* c = NULL;
	int wordlen = testBitlen / WORD_UNIT;
	big_gen_rand(&a, NON_NEGATIVE, wordlen);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	clock_t start = clock();
	for (int i = 0; i < testCase; i++)
	{
		big_multiplication(&c, a, b);
	}
	clock_t end = clock();
	float dif = (float)(end - start) / CLOCKS_PER_SEC;
	dif /= testCase;
	dif *= 10000;
	printf("%.5f\n", dif);

	big_delete(&a);
	big_delete(&b);
	big_delete(&c);
}
void squaringTimeTest()
{
	bigint* a = NULL;
	bigint* c = NULL;
	int wordlen = testBitlen / WORD_UNIT;
	big_gen_rand(&a, NON_NEGATIVE, wordlen);
	clock_t start = clock();
	for (int i = 0; i < testCase; i++)
	{
		big_squaring(&c, a);
	}
	clock_t end = clock();
	float dif = (float)(end - start) / CLOCKS_PER_SEC;
	dif /= testCase;
	dif *= 10000;
	printf("%.5f\n", dif);

	big_delete(&a);
	big_delete(&c);
}
void divisionTimeTest()
{
	bigint* a = NULL;
	bigint* b = NULL;
	bigint* q = NULL;
	bigint* r = NULL;
	int wordlen = testBitlen / WORD_UNIT;
	big_gen_rand(&a, NON_NEGATIVE, wordlen * 2);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	clock_t start = clock();
	for (int i = 0; i < 10; i++)
	{
		big_division(&q, &r, a, b);
	}
	clock_t end = clock();
	float dif = (float)(end - start) / CLOCKS_PER_SEC;
	dif /= 10;
	dif *= 10000;
	printf("%.5f\n", dif);

	big_delete(&a);
	big_delete(&b);
	big_delete(&q);
	big_delete(&r);
}
void modTimeTest()
{
	bigint* a = NULL;
	bigint* b = NULL;
	bigint* c = NULL;
	int wordlen = testBitlen / WORD_UNIT;
	big_gen_rand(&a, NON_NEGATIVE, wordlen * 2);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	clock_t start = clock();
	for (int i = 0; i < testCase; i++)
	{
		big_mod(&c, a, b);
	}
	clock_t end = clock();
	float dif = (float)(end - start) / CLOCKS_PER_SEC;
	dif /= testCase;
	dif *= 10000;
	printf("%.5f\n", dif);

	big_delete(&a);
	big_delete(&b);
	big_delete(&c);
}
void mod_expTimeTest()
{
	bigint* a = NULL;
	bigint* b = NULL;
	bigint* n = NULL;
	bigint* c = NULL;
	int wordlen = testBitlen / WORD_UNIT;
	int t = 1;
	big_gen_rand(&a, NON_NEGATIVE, wordlen);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	big_gen_rand(&n, NON_NEGATIVE, wordlen);
	clock_t start = clock();
	for (int i = 0; i < t; i++)
	{
		big_mod_exp(&c, a, n, b);
	}
	clock_t end = clock();
	float dif = (float)(end - start) / CLOCKS_PER_SEC;
	dif /= t;
	dif *= 10000;
	printf("%.5f\n", dif);

	big_delete(&a);
	big_delete(&b);
	big_delete(&n);
	big_delete(&c);
}