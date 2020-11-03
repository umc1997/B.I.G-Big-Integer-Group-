#ifndef BIGINT_H
#define BIGINT_H

#define WORD_UNIT 64 //8 32 64
#define NON_NEGATIVE 0
#define NEGATIVE 1

#define EQUAL 0
#define BIGGER 1
#define SMALLER -1

#if WORD_UNIT == 8 
#define word unsigned char
#elif WORD_UNIT == 32
#define word unsigned int
#elif WORD_UNIT == 64
#define word unsigned long long
#endif


typedef struct {
	int sign;
	int wordlen;
	word* a;
}bigint;


#endif