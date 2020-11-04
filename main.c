//memory leak detecter #include <vld.h> 
#include <stdio.h>
#include "CoreOperation.h"

void showProcessHex(bigint* a, bigint* b, bigint* c, char op);
void showProcessHexDiv(bigint* a, bigint* b, bigint* q, bigint* r);

int main()
{

	srand((unsigned)time(NULL));
	bigint* a = NULL;
	bigint* b = NULL;
	bigint* q = NULL;
	bigint* r = NULL;

	for (int i = 0; i < 100; i++)
	{
		int aSign = rand() % 2;
		int bSign = NON_NEGATIVE;
		int aWordlen = rand() % 20 + 1;
		int bWordlen = rand() % aWordlen + 1;
		big_gen_rand(&a, aSign, aWordlen);
		big_gen_rand(&b, bSign, bWordlen);
		if (big_is_zero(b))
			continue;

		big_division(&q, &r, a, b);
		showProcessHexDiv(a, b, q, r);

		/*
		show_by_dec test
		printf("a = ");
		big_show_bin(a);
		printf("\n");
		printf("b = ");
		big_show_dec(a);	
		printf("\n");
		printf("print(a == b)\n");*/
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
	printf("print(\"True\")\n");
	printf("else:\n\t");
	printf("print(hex(a//b))\n");
}
void showProcessHexDiv(bigint* a, bigint* b, bigint* q, bigint* r)
{
	printf("a = ");
	big_show_hex(a);
	printf("\n");
	printf("b = ");
	big_show_hex(b);
	printf("\n");
	printf("q = ");
	big_show_hex(q);
	printf("\n");
	printf("r = ");
	big_show_hex(r);
	printf("\n");
	printf("if a == b * q + r ");
	printf(":\n\t");
	printf("print(\"True\")\n");
	printf("else:\n");
	printf("\tprint(hex(a // b))\n");
	printf("\tprint(hex(a - b * (a // b)))\n");
}