//memory leak detector #include <vld.h> 
#include <stdio.h>
#include <time.h>
#include "CoreOperation.h"
#include "RSA.h"
#include "test.h"

int main()
{
	srand((unsigned)time(NULL));
	
	//Answer Test
	//printf("print(\"%d-bit Test\")\n", WORD_UNIT);

	//additionTest();
	//subtractionTest();
	//multiplicationTest();
	//squaringTest();
	//divisionTest();
	//mod_expTest();
	
	//Time Test
	
	//for (int i = 0; i < 5; i++)
	//	additionTimeTest();
	//printf("\n");
	//for (int i = 0; i < 5; i++)
	//	subtractionTimeTest();
	//printf("\n");
	/*for (int i = 0; i < 5; i++)
		multiplicationTimeTest();
	printf("\n");
	for (int i = 0; i < 5; i++)
		squaringTimeTest();
	printf("\n");*/
	/*for (int i = 0; i < 5; i++)
		divisionTimeTest();
	printf("\n");
	*/
	//mod_expTimeTest();

	RSATest();
	
	return 0;
}