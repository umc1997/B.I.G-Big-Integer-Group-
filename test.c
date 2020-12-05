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
void substractionTest()
{
	bigint* a = NULL;
	bigint* b = NULL;
	bigint* c = NULL;
	int wordlen = testBitlen / WORD_UNIT;

	printf("print(\"Substraction Test\")\n");
	printf("flag = 1\n");
	// c = a - b
	// test 1 : a = 0
	big_set_zero(&a);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	big_substraction(&c, a, b);
	showProcess(c, a, b, '-');

	// test 2 : b = 0
	big_gen_rand(&a, NON_NEGATIVE, wordlen);
	big_set_zero(&b);
	big_substraction(&c, a, b);
	showProcess(c, a, b, '-');

	// test 3 : a = b
	big_gen_rand(&a, NON_NEGATIVE, wordlen);
	big_assign(&b, a);
	big_substraction(&c, a, b);
	showProcess(c, a, b, '-');

	// test 4 : sign test
	//			(a > b > 0)  (b > a > 0)
	big_gen_rand(&a, NON_NEGATIVE, wordlen);
	big_gen_rand(&b, NON_NEGATIVE, wordlen - 1);
	big_substraction(&c, a, b);
	showProcess(c, a, b, '-');

	big_gen_rand(&a, NON_NEGATIVE, wordlen - 1);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	big_substraction(&c, a, b);
	showProcess(c, a, b, '-');

	//			(0 > a > b) (0 > b > a)
	big_gen_rand(&a, NEGATIVE, wordlen);
	big_gen_rand(&b, NEGATIVE, wordlen - 1);
	big_substraction(&c, a, b);
	showProcess(c, a, b, '-');

	big_gen_rand(&a, NEGATIVE, wordlen - 1);
	big_gen_rand(&b, NEGATIVE, wordlen);
	big_substraction(&c, a, b);
	showProcess(c, a, b, '-');

	//			(a > 0 & b < 0) (a < 0 & b > 0)   
	big_gen_rand(&a, NON_NEGATIVE, wordlen);
	big_gen_rand(&b, NEGATIVE, wordlen);
	big_substraction(&c, a, b);
	showProcess(c, a, b, '-');

	big_gen_rand(&a, NEGATIVE, wordlen);
	big_gen_rand(&b, NON_NEGATIVE, wordlen);
	big_substraction(&c, a, b);
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
		big_substraction(&c, a, b);
		showProcess(c, a, b, '-');
	}

	big_delete(&a);
	big_delete(&b);
	big_delete(&c);
	printf("if flag :\n");
	printf("\tprint(\"Substraction Test Passed\")\n");
	printf("else:\n");
	printf("\tprint(\"Substraction Test Failed\")\n");
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

void additionTimeTest();
void substractionTimeTest();
void multiplicationTimeTest();
void squaringTimeTest();
void divisionTimeTest();
void mod_expTimeTest();