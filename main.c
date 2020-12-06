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
	//substractionTest();
	//multiplicationTest();
	//squaringTest();
	//divisionTest();
	//mod_expTest();
	

	/*
	//Time Test
	*/
	multiplicationTimeTest();
	squaringTimeTest();
	divisionTimeTest();
	//modTimeTest();
	//mod_expTimeTest();

	//RSATest();
	
	return 0;
}