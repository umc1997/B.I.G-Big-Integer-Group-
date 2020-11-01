//memory leak detecter 
#include <vld.h> 
#include <stdio.h>
#include "CoreOperation.h"

void showProcessDec(bigint* a, bigint* b, bigint* c, char op);
void showProcessHex(bigint* a, bigint* b, bigint* c, char op);
void showBigint(bigint* a, int needBracket);
int main()
{
	
	srand((unsigned)time(NULL));
	bigint* a = NULL;
	bigint* b = NULL;
	bigint* mul = NULL;

	//one number is 0
	printf("Test 1\n");
	big_set_zero(&a);
	word words[5] = { 0x01,0x02,0x03,0x04,0x05 };
	big_set_by_array(&b, NON_NEGATIVE, words, 5);
	big_multiplication(&mul, a, b);
	showProcessDec(a, b, mul, '*');
	printf("\n");

	//one number is 1
	printf("Test 2\n");
	big_set_one(&a);
	big_set_by_string(&b, NEGATIVE, "123456789abcdef", 16);
	big_multiplication(&mul, a, b);

	showProcessDec(a, b, mul, '*');
	printf("\n");

	//one number is -1
	printf("Test 3\n");
	big_set_one(&a);
	big_flip_sign(&a);
	big_gen_rand(&b, NEGATIVE, 2);
	big_multiplication(&mul, a, b);

	showProcessDec(a, b, mul, '*');
	printf("\n");

	//sign test
	printf("Test 4\n");
	int sign1[4] = { NON_NEGATIVE,NON_NEGATIVE,NEGATIVE,NEGATIVE };
	int sign2[4] = { NON_NEGATIVE,NEGATIVE,NON_NEGATIVE,NEGATIVE };
	for (int i = 0; i < 4; i++)
	{
		printf("Operation %d\n", i + 1);
		big_gen_rand(&a, sign1[i], 2);
		big_gen_rand(&b, sign2[i], 2);
	
		big_multiplication(&mul, a, b);
		showProcessDec(a, b, mul, '*');
		printf("\n");
	}

	//random number operation
	printf("Test 5\n");
	for (int i = 0; i < 10; i++) {

		printf("Operation %d\n", i + 1);
		int aSign = rand() % 2;
		int bSign = rand() % 2;
		int aLength = rand() % 20 + 1;
		int bLength = rand() % 20 + 1;

		big_gen_rand(&a, aSign, aLength);
		big_gen_rand(&b, bSign, bLength);
		
		big_multiplication(&mul, a, b);

		showProcessHex(a, b, mul, '*');
	}
	
	big_delete(&a);
	big_delete(&b);
	big_delete(&mul);
	
	return 0;
}
void showProcessDec(bigint* a, bigint* b, bigint* c, char op)
{
	showBigint(a, 1);
	printf(" %c ", op);
	showBigint(b, 1);
	printf(" = ");
	showBigint(c, 0);
	printf("\n");
}
void showProcessHex(bigint* a, bigint* b, bigint* c, char op)
{
	printf("a: ");
	big_show_hex(a);
	printf("b: ");
	big_show_hex(b);
	printf("a %c b = ", op);
	big_show_hex(c);
	printf("\n");
}
void showBigint(bigint* a, int needBracket)
{
	int wordlen = a->wordlen;
	if (needBracket && a->sign)
		printf("(");
	if (a->sign)
		printf("-");
	unsigned long long output = 0;
	for (int i = wordlen - 1; i >= 0; i--)
	{
#if WORD_UNIT==8
		output <<= 8;
#elif WORD_UNIT==32
		output <<= 32;
#elif WORD_UNIT==64
		output <<= 64;
#endif
		output += (a->a[i]);
	}
	printf("%llu", output);
	if (needBracket && a->sign)
		printf(")");
}