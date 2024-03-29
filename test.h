#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <time.h>
#include "CoreOperation.h"
#include "RSA.h"
#include "BigInt.h"

#define testBitlen 1024
#define testCase 1000

void additionTest();
void subtractionTest();
void multiplicationTest();
void squaringTest();
void divisionTest();
void mod_expTest();
void RSATest();
void decimalTest();
void gcdTest();
void modular_inverseTest();
void primeTest();

void additionTimeTest();
void subtractionTimeTest();
void multiplicationTimeTest();
void squaringTimeTest();
void divisionTimeTest();
void modTimeTest();
void mod_expTimeTest();

#endif