#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

#include <stdio.h>

#define SUCCESS 0x00000000
// sys error
#define FAIL_OUT_OF_RANGE 0x00010001
#define FAIL_OUT_OF_MEMORY  0x00010002
#define FAIL_NULL 0x00010003
// invalid error
#define FAIL_INVALID_BASE 0x00020001
#define FAIL_INVALID_CHARACTER 0x00020002

typedef int ErrorMessage;

void printErrorMessage(ErrorMessage msg);


#endif