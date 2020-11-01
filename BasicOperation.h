#ifndef BASICOPERATION_H
#define BASICOPERATION_H
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "BigInt.h"
#include "ErrorMessage.h"
#define byte unsigned char

#define MAX(A,B) A>B? A:B

//generate random numbers and fill word array 
ErrorMessage array_rand(word* dst, int wordlen);
//fill word array with value
ErrorMessage array_fill(word* arr, int arrlen, int val);
//convert hex char to int
unsigned int hex2int(char c);
//check if char is valid char in base
int IsValidChar(char c, int base);
//single precision multiplication : A * B = C1 * W + C0  (0 <= A,B,C0,C1 < W)
ErrorMessage wordMultiplication(word* C1, word* C0, word A, word B);


#endif