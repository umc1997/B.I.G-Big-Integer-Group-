#ifndef BASICOPERATION_H
#define BASICOPERATION_H
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BigInt.h"
#include "ErrorMessage.h"
#define byte unsigned char
#define MAX(A,B) A>B? A:B

//generate random numbers and fill word array 
ErrorMessage array_rand(word* dst, int wordlen);
//fill word array with value
ErrorMessage array_fill(word* arr, int arrlen, int val);
//reverse string
void reverseStr(char* str);
//convert digit to charnum
char digit2char(word a);
//convert hex char to int
unsigned int hex2int(char c);
//check if char is valid char in base
bool isValidChar(char c, int base);
//single precision multiplication : A * B = C1 * W + C0  (0 <= A,B,C0,C1 < W)
ErrorMessage wordMultiplication(word* C1, word* C0, word A, word B);
//single precision squaring : A * A = C1 * W + C0 (0 <= A, C0, C1 < W)
ErrorMessage wordSquaring(word* C1, word* C0, word A);
//2 - words long division : find Q such that A(A1W+A0) = BQ + R (0 <= R < B)
ErrorMessage wordLongDivision(word* Q, word A1, word A0, word B);

#endif