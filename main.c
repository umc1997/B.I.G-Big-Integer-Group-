//memory leak detecter #include <vld.h> 
#include <stdio.h>
#include <time.h>
#include "CoreOperation.h"
#define testMode 0 // Vaild answer checking = 0, Time checking = 1

void showProcessHex(bigint* a, bigint* b, bigint* c);

int main()
{
	srand((unsigned)time(NULL));

	bigint* a = NULL;
	bigint* b = NULL;
	bigint* c = NULL;

	int testCase = 100;
	int bit = 1024;
	int aWordlen = bit / WORD_UNIT; // 1024 - bit
	int bWordlen = bit / WORD_UNIT; // 1024 - bit

	big_set_by_string(&b, NON_NEGATIVE, "10001", 16);

#if testMode == 1
	for (int i = 0; i < 5; i++) {
		clock_t start = clock();
#endif
		
		for (int t = 0; t < testCase; t++) {
			/* generate random number */
			big_gen_rand(&a, NON_NEGATIVE, aWordlen);
			//big_gen_rand(&b, NON_NEGATIVE, bWordlen);
			
			/* operation */
			big_mod_inverse(&c, a, b);
			
#if testMode == 0
			/* show */
			showProcessHex(a, b, c);
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
	big_delete(&c);
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