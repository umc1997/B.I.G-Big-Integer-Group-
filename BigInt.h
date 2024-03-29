#ifndef BIGINT_H
#define BIGINT_H

#include "config.h"

#define NON_NEGATIVE 0
#define NEGATIVE 1

#define EQUAL 0
#define BIGGER 1
#define SMALLER -1

#if WORD_UNIT == 8 
#define word unsigned char
#define WORD_MASK 0xff
#elif WORD_UNIT == 32
#define word unsigned int
#define WORD_MASK 0xffffffff
#elif WORD_UNIT == 64
#define word unsigned long long
#define WORD_MASK 0xffffffffffffffff
#endif


typedef struct {
	int sign;
	int wordlen;
	word* a;
}bigint;

#endif