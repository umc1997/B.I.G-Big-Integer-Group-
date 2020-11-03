//memory leak detecter #include <vld.h> 
#include <stdio.h>
#include "CoreOperation.h"

void showProcessHex(bigint* a, bigint* b, bigint* c, char op);
void showBigint(bigint* a, int needBracket);

int main()
{

	srand((unsigned)time(NULL));
	bigint* a = NULL;
	bigint* b = NULL;
	bigint* q = NULL;
	bigint* r = NULL;

	for (int i = 0; i < 10; i++)
	{
		big_gen_rand(&a, NON_NEGATIVE, 2);
		big_gen_rand(&b, NON_NEGATIVE, 1);
	
		big_division(&q, &r, a, b);
		showProcessHex(a, b, q, '/');
		
	}
	big_delete(&a);
	big_delete(&b);
	big_delete(&q);
	big_delete(&r);
	
	return 0;
}

void showProcessHex(bigint* a, bigint* b, bigint* c, char op)
{
	printf("a = ");
	big_show_hex(a);
	printf("\n");
	printf("b = ");
	big_show_hex(b);
	printf("\n");
	printf("if a %c/ b == ", op);
	big_show_hex(c);
	printf(":\n\t");
	printf("print(1)\n");
	printf("else:\n\t");
	printf("print(hex(a//b))\n");
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