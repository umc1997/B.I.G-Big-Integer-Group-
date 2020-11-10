//memory leak detecter #include <vld.h> 
#include <stdio.h>
#include "CoreOperation.h"

void showProcessHexModExp(bigint* a, bigint* n, bigint* b, bigint* c);

int main()
{
	srand((unsigned)time(NULL));

	clock_t start, end;
	bigint* a = NULL;
	bigint* b = NULL;
	bigint* n = NULL;
	bigint* c = NULL;
	int testCase = 1000;
	int aWordlen = 10;
	int bWordlen = 10;
	big_set_by_string(&n, NON_NEGATIVE, "10001", 16);

	for (int i = 0; i < 5; i++) {
		start = clock();
		for (int t = 0; t < testCase; t++) {

			//generate random big integer
			big_gen_rand(&a, NON_NEGATIVE, aWordlen);
			big_gen_rand(&b, NON_NEGATIVE, bWordlen);
			while (big_is_zero(b))
				big_gen_rand(&b, NON_NEGATIVE, bWordlen);

			//mod_exp
			big_mod_exp(&c, a, n, b);

		}
		end = clock();
		float dif = (float)(end - start) / CLOCKS_PER_SEC;
		printf("%f\n", dif / testCase);
	}

	big_delete(&a);
	big_delete(&b);
	big_delete(&c);
	big_delete(&n);

	return 0;
}
void showProcessHexModExp(bigint* a, bigint* n, bigint* b, bigint* c)
{
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