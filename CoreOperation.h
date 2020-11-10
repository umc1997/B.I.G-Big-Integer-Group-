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

#define karaFlag 2

#define EXPMODMODE 1 // 1 = L2R, 2 = R2L, 3 = MS

// constructor
ErrorMessage big_new(bigint** x, int sign, int wordlen);

// deleter
ErrorMessage big_delete(bigint** x);

// setter
// set big int with input array (base = 10)
ErrorMessage big_set_by_array(bigint** x, int sign, word* a, int wordlen);
// set big int with input string (base = 2 , base = 10)
ErrorMessage big_set_by_string(bigint** x, int sign, char* str, int base);
// set zero
ErrorMessage big_set_zero(bigint** x);
// set one
ErrorMessage big_set_one(bigint** x);

// getter
// return wordlenghth of bigint x
int big_get_wordlen(bigint* x);
// return bitlength of bigint x
int big_get_bitlen(bigint* x);
// return certain bit of bigint x
int big_get_bit(bigint* x, int bitindex);
// return sign of bigint x
int big_get_sign(bigint* x);

// checker
// return x == 0 (0 = x is not zero , 1 = x is zero)
bool big_is_zero(bigint* x);
// return x == 1 (0 = x is not one , 1 = x is one)
bool big_is_one(bigint* x);
// return x == -1 (0 = x is not minus one, 1 = x is minus one)
bool big_is_minus_one(bigint* x);
// return x is odd
bool big_is_odd(bigint* x);
// return x is even
bool big_is_even(bigint* x);

// shower
// show big int with hex
ErrorMessage big_show_hex(bigint* x);
// show big int with dec
ErrorMessage big_show_dec(bigint* x);
// show big int with dec for each word
ErrorMessage big_show_dec_for_each_word(bigint* x);
// show big int with bin
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
// compare two big int
int big_compare(bigint* x, bigint* y);
// compare two big int ignore sign
int big_compareABS(bigint* x, bigint* y);

// shift & reduction operation
// operating src and save it to dst
// left bit shift ( *dst = (src << count) )
ErrorMessage big_bit_left_shift(bigint** dst, bigint* src, int count);
// right bit shift ( *dst = (src >> count) )
ErrorMessage big_bit_right_shift(bigint** dst, bigint* src, int count);
// reduction ( *dst = src % (2^count) )
ErrorMessage big_bit_reduction(bigint** dst, bigint* src, int count);
// left bit shift ( *dst = (src << (count * word_unit) ))
ErrorMessage big_word_left_shift(bigint** dst, bigint* src, int count);
// right bit shift ( *dst = (src >> (count * word_unit) ))
ErrorMessage big_word_right_shift(bigint** dst, bigint* src, int count);
// reduction ( *dst = src % (word_unit^count) )
ErrorMessage big_word_reduction(bigint** dst, bigint* src, int count);

// addition z = x + y
// add x and y, assign to z
ErrorMessage big_addition(bigint** z, bigint* x, bigint* y);
// add x and y ignore sign, assign to z (input:x > 0, y > 0, z != x ,z != y) 
ErrorMessage big_additionABS(bigint** z, bigint* x, bigint* y);

// substraction z = x - y
// substract y from x, assign to z
ErrorMessage big_substraction(bigint** z, bigint* x, bigint* y);
// substract y from x ignore sign, assign to z  (input: x > 0 ,y > 0, x > y, z != x ,z != y)
ErrorMessage big_substractionABS(bigint** z, bigint* x, bigint* y);

// multiplication z = x * y
// multiply x and y , assign to z
ErrorMessage big_multiplication(bigint** z, bigint* x, bigint* y);
// multiply x and y , assign to z  (input: x > 0, y > 0) 
ErrorMessage big_multiplicationABS(bigint** z, bigint* x, bigint* y);
// multiply x and y ignore sign, assign to z (using schoolbook algorithm) (use for wordlen(x) or wordlen(y) <= karaFlag) 
ErrorMessage big_multiplicationSchoolBook(bigint** z, bigint* x, bigint* y);
// multiply x and y ignore sign, assign to z using karatsuba algorithm (use for wordlen(x) or wordlen(y) > karaFlag) 
ErrorMessage big_multiplicationKaratsuba(bigint** z, bigint* x, bigint* y);
// multiply bigint x and word y, assign to z ( y range = word range )
ErrorMessage big_multiplicationConst(bigint** z, bigint* x, word y);

// squaring z = x * x
// square x , assign to z
ErrorMessage big_squaring(bigint** z, bigint* x);
// square x (x > 0) , assign to z (x != z)
ErrorMessage big_squaringABS(bigint** z, bigint* x);
// square x , assign to z (using schoolbook algorithm) (use for wordlen(x) or wordlen(y) <= karaFlag) 
ErrorMessage big_squaringSchoolBook(bigint** z, bigint* x);
// square x , assign to z using karatsuba algorithm (use for wordlen(x) or wordlen(y) > karaFlag) 
ErrorMessage big_squaringKaratsuba(bigint** z, bigint* x);

// division x = y * q + r (0 <= r < y)
// divide x with y , calculate q and r ( 0 <= r < y) , assign to q and r (input: y > 0)
ErrorMessage big_division(bigint** q, bigint** r, bigint* x, bigint* y);
// divide x with y , calculate q and r ( 0 <= r < y) , assign to q and r (input:x > 0, y > 0, x > y)
ErrorMessage big_divisionABS(bigint** q, bigint** r, bigint* x, bigint* y);
// divide x with y , calculate q and r ( 0 <= r < y) , assign to q and r (input:x > 0, y > 0, y <= x < y * W)
ErrorMessage big_divisionCore(word* q, bigint** r, bigint* x, bigint* y);
// divide x with y , calculate q and r ( 0 <= r < y) , assign to q and r (input:x > 0, y > 0, y <= x < y * W)
ErrorMessage big_divisionCoreCore(word* q, bigint** r, bigint* x, bigint* y);

// modular z = x mod y
ErrorMessage big_mod(bigint** z, bigint* x, bigint* y);

// modular exponeniation z = x ^ n mod y
// compute x ^ n and modular y, assign to z (input: n >= 0 , y > 0)
ErrorMessage big_mod_exp(bigint** z, bigint* x, bigint* n, bigint* y);
// compute x ^ n and modular y, assign to z (input: n >= 0 , x > 0. y > 0)
ErrorMessage big_mod_expABS(bigint** z, bigint* x, bigint* n, bigint* y);
// compute x ^ n and modular y, assign to z using left to right algorithm(input: n > 0 , x > 0. y > 0)
ErrorMessage big_mod_expL2R(bigint** z, bigint* x, bigint* n, bigint* y);
// compute x ^ n and modular y, assign to z using rigth to right algorithm(input: n > 0 , x > 0. y > 0)
ErrorMessage big_mod_expR2L(bigint** z, bigint* x, bigint* n, bigint* y);
// compute x ^ n and modular y, assign to z using multiply and squaring algorithm(input: n > 0 , x > 0. y > 0)
ErrorMessage big_mod_expMS(bigint** z, bigint* x, bigint* n, bigint* y);


#endif