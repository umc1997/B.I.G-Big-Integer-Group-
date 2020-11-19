#ifndef COREOPERATION_H
#define COREOPERATION_H
#include "BigInt.h"
#include "ErrorMessage.h"
#include "BasicOperation.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define EQUAL 0
#define BIGGER 1
#define SMALLER -1

#define karaFlag 4

#define EXPMODMODE 1 // 1 = L2R, 2 = R2L, 3 = MS

// constructor
ErrorMessage big_new(bigint** x, int sign, int wordlen);

// deleter
ErrorMessage big_delete(bigint** x);

// setter
ErrorMessage big_set_by_array(bigint** x, int sign, word* a, int wordlen);
ErrorMessage big_set_by_string(bigint** x, int sign, char* str, int base);
ErrorMessage big_set_zero(bigint** x);
ErrorMessage big_set_one(bigint** x);

// getter
int big_get_wordlen(bigint* x);
int big_get_bitlen(bigint* x);
int big_get_bit(bigint* x, int bitindex);
int big_get_sign(bigint* x);

// checker
bool big_is_zero(bigint* x);
bool big_is_one(bigint* x);
bool big_is_minus_one(bigint* x);
bool big_is_odd(bigint* x);
bool big_is_even(bigint* x);

// shower
ErrorMessage big_show_hex(bigint* x);
ErrorMessage big_show_dec(bigint* x);
ErrorMessage big_show_dec_for_each_word(bigint* x);
ErrorMessage big_show_bin(bigint* x);

// erase useless 0 words in front of bigint 
ErrorMessage big_refine(bigint* x);

// assign src bigint to dst bigint
ErrorMessage big_assign(bigint** dst, bigint* src);

// generate random bigint x
ErrorMessage big_gen_rand(bigint** x, int sign, int wordlen);

// filp sign of bigint x to inverse
ErrorMessage big_flip_sign(bigint** x);

// compare x with y (1 = x is bigger, 0 = two numbers are equal, -1 = y is bigger)
int big_compare(bigint* x, bigint* y);
static int big_compareABS(bigint* x, bigint* y);

// shift & reduction operation
ErrorMessage big_bit_left_shift(bigint** dst, bigint* src, int count);
ErrorMessage big_bit_right_shift(bigint** dst, bigint* src, int count);
ErrorMessage big_bit_reduction(bigint** dst, bigint* src, int count);
ErrorMessage big_word_left_shift(bigint** dst, bigint* src, int count);
ErrorMessage big_word_right_shift(bigint** dst, bigint* src, int count);
ErrorMessage big_word_reduction(bigint** dst, bigint* src, int count);

// addition z = x + y
ErrorMessage big_addition(bigint** z, bigint* x, bigint* y);
static ErrorMessage big_additionABS(bigint** z, bigint* x, bigint* y);

// substraction z = x - y
ErrorMessage big_substraction(bigint** z, bigint* x, bigint* y);
static ErrorMessage big_substractionABS(bigint** z, bigint* x, bigint* y);

// multiplication z = x * y
ErrorMessage big_multiplication(bigint** z, bigint* x, bigint* y);
static ErrorMessage big_multiplicationABS(bigint** z, bigint* x, bigint* y);
static ErrorMessage big_multiplicationSchoolBook(bigint** z, bigint* x, bigint* y);
static ErrorMessage big_multiplicationKaratsuba(bigint** z, bigint* x, bigint* y);
ErrorMessage big_multiplicationConst(bigint** z, bigint* x, word y);

// squaring z = x * x
ErrorMessage big_squaring(bigint** z, bigint* x);
static ErrorMessage big_squaringABS(bigint** z, bigint* x);
static ErrorMessage big_squaringSchoolBook(bigint** z, bigint* x);
static ErrorMessage big_squaringKaratsuba(bigint** z, bigint* x);

// division x = y * q + r (0 <= r < y)
ErrorMessage big_division(bigint** q, bigint** r, bigint* x, bigint* y);
static ErrorMessage big_divisionABS(bigint** q, bigint** r, bigint* x, bigint* y);
static ErrorMessage big_divisionCore(word* q, bigint** r, bigint* x, bigint* y);
static ErrorMessage big_divisionCoreCore(word* q, bigint** r, bigint* x, bigint* y);

// modular z = x mod y
ErrorMessage big_mod(bigint** z, bigint* x, bigint* y);

// modular inverse
ErrorMessage big_mod_inverse(bigint** z, bigint* x, bigint* y);

// modular exponentiation z = x ^ n mod y
ErrorMessage big_mod_exp(bigint** z, bigint* x, bigint* n, bigint* y);
static ErrorMessage big_mod_expABS(bigint** z, bigint* x, bigint* n, bigint* y);
static ErrorMessage big_mod_expL2R(bigint** z, bigint* x, bigint* n, bigint* y);
static ErrorMessage big_mod_expR2L(bigint** z, bigint* x, bigint* n, bigint* y);
static ErrorMessage big_mod_expMS(bigint** z, bigint* x, bigint* n, bigint* y);

// great common divisor of x and y , z = gcd(x, y)
ErrorMessage big_gcd(bigint** z, bigint* x, bigint* y);
static ErrorMessage big_gcdRecursive(bigint** z, bigint* x, bigint* y);

// extended euclidean algorithm
ErrorMessage big_xgcd(bigint** d, bigint** x, bigint** y, bigint* a, bigint* b);
static ErrorMessage big_xgcdRecursive(bigint** d, bigint** x, bigint** y, bigint* a, bigint* b);
#endif