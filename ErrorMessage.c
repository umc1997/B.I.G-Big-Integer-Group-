#include "ErrorMessage.h"

void printErrorMessage(ErrorMessage msg) {
	switch (msg)
	{
	case(SUCCESS):
		break;
	case(FAIL_OUT_OF_RANGE):
		printf("Error: Segmentation fault (OUT_OF_RANGE).\n");
		break;
	case(FAIL_OUT_OF_MEMORY):
		printf("Error: There is no memory to allocate (OUT_OF_MEMORY).\n");
		break;
	case(FAIL_NULL):
		printf("Error: The Big Number is NULL (NULL).\n");
		break;
	case(FAIL_INVALID_BASE):
		printf("Error: Base is not provided (INVALID_BASE).\n");
		break;
	case(FAIL_INVALID_CHARACTER):
		printf("Error: Please enter valid character (<bin: 0, 1> ,<dec:0 ~ 9>, <hex:0 ~ f>) (INVALID_CHARACTER) .\n");
	case(FAIL_INVALID_DIVISOR):
		printf("Error: Divisor must be a positive number (INVALID_DIVISOR) .\n");
	case(FAIL_INVALID_EXPONENT):
		printf("Error: Exponent must be a non-negative number (INVALID_EXPONENT) .\n");
		break;
	}
}
