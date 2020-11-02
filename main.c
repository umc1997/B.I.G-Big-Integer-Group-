//memory leak detecter #include <vld.h> 
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

	//big_set_by_string(&a, NON_NEGATIVE, "6c51f99580b09439deae", 16);
	//big_set_by_string(&b, NON_NEGATIVE, "42526518a0aca56a94a4", 16);

	//big_multiplication(&mul, a, b);
	//showProcessHex(a, b, mul, '*');
	for (int i = 0; i < 100; i++)
	{
		big_gen_rand(&a, NON_NEGATIVE, 10);
		big_gen_rand(&b, NON_NEGATIVE, 10);

		
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
	printf("a = ");
	big_show_hex(a);
	printf("\n");
	printf("b = ");
	big_show_hex(b);
	printf("\n");
	printf("if a %c b == ", op);
	big_show_hex(c);
	printf(":\n\t");
	printf("print(1)\n");
	printf("else:\n\t");
	printf("print(hex(a*b))\n");
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