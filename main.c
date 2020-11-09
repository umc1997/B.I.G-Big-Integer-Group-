//memory leak detecter #include <vld.h> 
#include <stdio.h>
#include "CoreOperation.h"

void showProcessHexSqu(bigint* a, bigint* c);
void showProcessHexDiv(bigint* a, bigint* b, bigint* q, bigint* r);

int main()
{
	bigint* a = NULL;
	bigint* b = NULL;
	bigint* n = NULL;
	bigint* c = NULL;
	for (int i = 0; i < 1000; i++) {

		int aWordlen = rand() % 20 + 1;
		int bWordlen = rand() % 20 + 1;

		big_gen_rand(&a, NON_NEGATIVE, aWordlen);
		big_set_by_string(&n, NON_NEGATIVE, "3", 16);
		big_gen_rand(&b, NON_NEGATIVE, bWordlen);

		big_mod_exp(&c, a, n, b);

		printf("a = ");
		big_show_hex(a);
		printf("\n");
		printf("n = ");
		big_show_hex(n);
		printf("\n");
		printf("b = ");
		big_show_hex(b);
		printf("\n");
		printf("c = ");
		big_show_hex(c);
		printf("\n");
		printf("if pow(a, n, b) == c");
		printf(":\n\t");
		printf("print(\"True\")\n");
		printf("else:\n\t");
		printf("print(\"False\")\n");
	}
	big_delete(&a);
	big_delete(&b);
	big_delete(&n);
	big_delete(&c);

	return 0;
}


void showProcessHexSqu(bigint* a, bigint* c)
{
	printf("a = ");
	big_show_hex(a);
	printf("\n");
	printf("if a ** 2 == ");
	big_show_hex(c);
	printf(":\n\t");
	printf("print(\"True\")\n");
	printf("else:\n\t");
	printf("print(a ** 2)\n");
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
	printf("if (a == b * q + r) and (b > r) ");
	printf(":\n\t");
	printf("print(\"True\")\n");
	printf("else:\n");
	printf("\tprint(hex(a // b))\n");
	printf("\tprint(hex(a - b * (a // b)))\n");
}