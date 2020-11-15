//memory leak detecter #include <vld.h> 
#include <stdio.h>
#include <time.h>
#include "CoreOperation.h"

#define testMode 0 // Vaild answer checking = 0, Time checking = 1

void showProcessHexModExp(bigint* a, bigint* n, bigint* b, bigint* c);

int main()
{
	srand((unsigned)time(NULL));

	bigint* a = NULL;
	bigint* b = NULL;
	bigint* i = NULL;

	int testCase = 10;
	//int bit = 1024;
	//int aWordlen = bit / WORD_UNIT; // 1024 - bit
	//int bWordlen = bit / WORD_UNIT; // 1024 - bit
	int aWordlen = 2;
	int bWordlen = 2;
	//big_set_by_string(&n, NON_NEGATIVE, "10001", 16);
#if testMode == 1
	for (int i = 0; i < 5; i++) {
		clock_t start = clock();
#endif
		
		big_set_by_string(&a, NON_NEGATIVE, "10001", 16);
		for (int t = 0; t < testCase; t++) {
			/* generate random number */
			bool flag = false;
			big_gen_rand(&b, NON_NEGATIVE, aWordlen);
			

			printf("a = ");
			big_show_dec(a);
			printf("b = ");
			big_show_dec(b);

			/* operation */
			big_mod_inverse(&i, a, b);

			printf("inverse = ");
			big_show_dec(i);

			bigint* tmp = NULL;
			big_multiplication(&tmp, a, i);
			big_mod(&tmp, tmp, b);

			printf("a * inverse mod b = ");
			big_show_dec(tmp);

			printf("\n");

#if testMode == 0
			/* show */
			
#endif
		}
#if testMode == 1
		clock_t end = clock();
		float dif = (float)(end - start) / CLOCKS_PER_SEC;
		printf("%f\n", dif  / testCase);
	}
#endif
	big_delete(&a);
	big_delete(&b);
	big_delete(&i);
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