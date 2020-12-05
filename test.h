#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <time.h>
#include "CoreOperation.h"
#include "RSA.h"
#include "BigInt.h"

#define testBitlen 2048
#define testCase 1000

void additionTest();
void substractionTest();
void multiplicationTest();
void squaringTest();
void divisionTest();
void mod_expTest();
void RSATest();

void additionTimeTest();
void substractionTimeTest();
void multiplicationTimeTest();
void squaringTimeTest();
void divisionTimeTest();
void mod_expTimeTest();

#endif