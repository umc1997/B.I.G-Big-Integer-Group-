//memory leak detecter #include <vld.h> 
#include <stdio.h>
#include <time.h>
#include "CoreOperation.h"

#define timeCheck 0 // no = 0, yes = 1

void showProcessHexModExp(bigint* a, bigint* n, bigint* b, bigint* c);

int main()
{
	srand((unsigned)time(NULL));

	bigint* a = NULL;
	bigint* b = NULL;
	bigint* n = NULL;
	bigint* c = NULL;

	int testCase = 1000;
	int bit = 1024;
	int aWordlen = bit / WORD_UNIT; // 1024 - bit
	int bWordlen = bit / WORD_UNIT; // 1024 - bit

	big_set_by_string(&n, NON_NEGATIVE, "10001", 16);

	for (int i = 0; i < 5; i++) {
#if timeCheck == 1
		clock_t start = clock();
#endif
		for (int t = 0; t < testCase; t++) {

			//generate random big integer
			big_gen_rand(&a, NON_NEGATIVE, aWordlen);
			big_gen_rand(&b, NON_NEGATIVE, bWordlen);
			while (big_is_zero(b))
				big_gen_rand(&b, NON_NEGATIVE, bWordlen);

			//mod_exp
			big_mod_exp(&c, a, n, b);

			//show
#if timeCheck == 0
			showProcessHexModExp(a, n, b, c);
#endif
		}
#if timeCheck == 1
		clock_t end = clock();
		float dif = (float)(end - start) / CLOCKS_PER_SEC;
		printf("%f\n", dif / testCase);
#endif
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
	printf("n = ");
	big_show_hex(n);
	printf("b = ");
	big_show_hex(b);
	printf("c = ");
	big_show_hex(c);
	printf("if pow(a, n, b) == c :\n\t");
	printf("print(\"True\")\n");
	printf("else:\n\t");
	printf("print(pow(a, n, b))\n");
}	