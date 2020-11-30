//memory leak detecter #include <vld.h> 
#include <stdio.h>
#include <time.h>
#include "CoreOperation.h"
#define testMode 1 // Vaild answer checking = 0, Time checking = 1

void showProcessHex(bigint* a, bigint* b, bigint* c);

int main()
{
	srand((unsigned)time(NULL));

	bigint* a = NULL;
	bigint* b = NULL;
	bigint* c = NULL;
	bigint* n = NULL;

	int testCase = 100;
	int bit = 1024;
	int aWordlen = bit / WORD_UNIT; // 1024 - bit
	int bWordlen = bit / WORD_UNIT; // 1024 - bit
	//int nWordlen = bit / WORD_UNIT; // 1024 - bit

#if testMode == 1
	for (int i = 0; i < 5; i++) {
		clock_t start = clock();
#endif
		for (int t = 0; t < testCase; t++) {
			/* generate random number */
			big_gen_rand(&a, NON_NEGATIVE, aWordlen);
		//	do
			//{
				big_gen_rand(&b, NON_NEGATIVE, bWordlen);
			//} while (big_is_zero(b));
			//big_gen_rand(&n, NON_NEGATIVE, nWordlen);

#if testMode == 0 
			printf("a = ");
			big_show_hex(a);
			printf("b = ");
			big_show_hex(b);
			//printf("n = ");
			//big_show_hex(n);
#endif

			/* operation */
			//big_mod_exp(&c, a, n, b);
			big_multiplication(&c, a, b);
			
#if testMode == 0
			/* show */
			//showProcessHex(a, b, c);
		
			printf("c = ");
			big_show_hex(c);
			printf("if pow(a,n,b) == c :\n\t");
			printf("print(\"True\")\n");
			printf("else:\n");
			printf("\tprint(\"False\")\n");
#endif
		}
#if testMode == 1
		clock_t end = clock();
		float dif = (float)(end - start) / CLOCKS_PER_SEC;
		dif /= testCase;
		dif *= 10000;
		printf("%.5f\n", dif);
	}
#endif
	big_delete(&a);
	big_delete(&b);
	big_delete(&c);
	big_delete(&n);
	
	return 0;
}
void showProcessHex(bigint* a, bigint* b, bigint* c)
{
	printf("a = ");
	big_show_hex(a);
	printf("b = ");
	big_show_hex(b);
	printf("c = ");
	big_show_hex(c);
	printf("if a * c %% b == 1 :\n\t");
	printf("print(\"True\")\n");
	printf("else:\n");
	printf("\tprint(\"False\")\n");
}