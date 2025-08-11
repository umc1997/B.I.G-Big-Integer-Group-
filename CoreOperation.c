#include "CoreOperation.h"

/* prototype */
static int big_compareABS(bigint* x, bigint* y);
static ErrorMessage big_refine(bigint* x);
static ErrorMessage big_additionABS(bigint** z, bigint* x, bigint* y);
static ErrorMessage big_subtractionABS(bigint** z, bigint* x, bigint* y);
static ErrorMessage big_multiplicationABS(bigint** z, bigint* x, bigint* y);
static ErrorMessage big_multiplicationSchoolBook(bigint** z, bigint* x, bigint* y);
static ErrorMessage big_multiplicationKaratsuba(bigint** z, bigint* x, bigint* y);
static ErrorMessage big_squaringABS(bigint** z, bigint* x);
static ErrorMessage big_squaringSchoolBook(bigint** z, bigint* x);
static ErrorMessage big_squaringKaratsuba(bigint** z, bigint* x);
static ErrorMessage big_divisionABS(bigint** q, bigint** r, bigint* x, bigint* y);
static ErrorMessage big_divisionCore(word* q, bigint** r, bigint* x, bigint* y);
static ErrorMessage big_divisionCoreCore(word* q, bigint** r, bigint* x, bigint* y);
static ErrorMessage big_mod_expABS(bigint** z, bigint* x, bigint* n, bigint* y);
static ErrorMessage big_mod_expL2R(bigint** z, bigint* x, bigint* n, bigint* y);
static ErrorMessage big_mod_expR2L(bigint** z, bigint* x, bigint* n, bigint* y);
static ErrorMessage big_mod_expMS(bigint** z, bigint* x, bigint* n, bigint* y);
static ErrorMessage big_gcdRecursive(bigint** z, bigint* x, bigint* y);
static ErrorMessage big_xgcdRecursive(bigint** d, bigint** x, bigint** y, bigint* a, bigint* b);
/* prototype */

/**
 * construct a bigint.
 *
 * \param x : address of bigint (can be NULL)
 * \param sign : sign of bigint (NON_NEGATIVE = 0, NEGATIVE = 1)
 * \param wordlen : word length of bigint (int type)
 * \return : ErrorMessage
 */
ErrorMessage big_new(bigint** x, int sign, int wordlen) {
	if (*x != NULL)
		big_delete(x);
	*x = (bigint*)malloc(sizeof(bigint));
	if (*x == NULL) return FAIL_OUT_OF_MEMORY;
	(*x)->sign = sign;
	(*x)->wordlen = wordlen;
	(*x)->a = (word*)calloc(wordlen, sizeof(word));
	if ((*x)->a == NULL) return FAIL_OUT_OF_MEMORY;
	return SUCCESS;
}
/**
 * delete the bigint.
 *
 * \param x : address of bigint (can be NULL)
 * \return : ErrorMessage
 */
ErrorMessage big_delete(bigint** x) {
	if (*x == NULL)
		return SUCCESS;
#ifdef ZEORIZE
	array_fill((*x)->a, (*x)->wordlen, 0);
#endif
	free((*x)->a);
	free(*x);
	*x = NULL;
	return SUCCESS;
}
/**
 * set bigint by word array.
 *
 * \param x : address of bigint (can be NULL)
 * \param sign : sign of bigint (NON_NEGATIVE = 0, NEGATIVE = 1)
 * \param a : pointer of word array
 * \param wordlen : word length of bigint (int type)
 * \return : ErrorMessage
 */
ErrorMessage big_set_by_array(bigint** x, int sign, word* a, int wordlen) {
	big_new(x, sign, wordlen);
	for (int i = 0; i < wordlen; i++)
		(*x)->a[i] = a[i];
	big_refine(*x);
	return SUCCESS;
}
/**
 * set bigint by string.
 *
 * \param x : address of bigint (can be NULL)
 * \param sign : sign of bigint (NON_NEGATIVE = 0, NEGATIVE = 1)
 * \param str : pointer of string
 * \param base : base of number string (provide 2, 10 and 16)
 * \return : ErrorMessage
 */
ErrorMessage big_set_by_string(bigint** x, int sign, char* str, int base) {
	if (base != 2 && base != 10 && base != 16)
		return FAIL_INVALID_BASE;
	int strLen = (int)strlen(str);
	for (int i = 0; i < strLen; i++)
	{
		char currentChar = *(str + i);
		if (!isValidChar(currentChar, base))
			return FAIL_INVALID_CHARACTER;

	}
	int cnt = 1;
	if (base == 10)
	{
		big_set_zero(x);

		//left to right	
		for (int i = 0; i < strLen; i++)
		{
			// mul
			big_multiplicationConst(x, *x, 10);
			// convert int to bigint
			word currentDigit[1] = { hex2int(*(str + i)) };
			bigint* tmp = NULL;
			big_set_by_array(&tmp, NON_NEGATIVE, currentDigit, 1);
			// add
			big_addition(x, *x, tmp);

			big_delete(&tmp);
		}
		(*x)->sign = sign;
	}
	else {
		if (base == 2)
		{
#if WORD_UNIT==8
			cnt = strLen >> 3;
			if (strLen != cnt << 3)
				cnt++;
#elif WORD_UNIT==32
			cnt = strLen >> 5;
			if (strLen != cnt << 5)
				cnt++;
#elif WORD_UNIT==64
			cnt = strLen >> 6;
			if (strLen != cnt << 6)
				cnt++;
#endif
		}
		else if (base == 16)
		{
#if WORD_UNIT==8
			cnt = strLen >> 1;
			if (strLen != cnt << 1)
				cnt++;
#elif WORD_UNIT==32
			cnt = strLen >> 3;
			if (strLen != cnt << 3)
				cnt++;
#elif WORD_UNIT==64
			cnt = strLen >> 4;
			if (strLen != cnt << 4)
				cnt++;
#endif
		}

		// alloc
		big_new(x, sign, cnt);

		// assign
		word sum = 0;
		int ptr = 0;
		int wordUnitOfStr = WORD_UNIT;
		if (base == 16)
			wordUnitOfStr >>= 2;
		for (int i = 0; i < strLen; i++)
		{
			sum *= base;
			sum += hex2int(*(str + i));
			if ((strLen - i) % wordUnitOfStr == 1)
			{
				(*x)->a[cnt - ptr - 1] = sum;
				ptr++;
				sum = 0;
			}
		}
	}

	big_refine(*x);
	return SUCCESS;
}
/**
 * show bigint by hexadecimal.
 *
 * \param x : address of bigint (can't be NULL)
 * \return : ErrorMessage
 */
ErrorMessage big_show_hex(bigint* x) {
	if (x == NULL)
		return FAIL_NULL;
	int wordlen = x->wordlen;
	if (x->sign == NEGATIVE)
		printf("-");
	printf("0x");
	for (int i = wordlen - 1; i >= 0; i--)
	{
#if WORD_UNIT==8
		printf("%02x", x->a[i]);
#elif WORD_UNIT==32
		printf("%08x", x->a[i]);
#elif WORD_UNIT==64
		printf("%016llx", x->a[i]);
#endif
	}
	printf("\n");
	return SUCCESS;
}
/**
 * show bigint by decimal.
 *
 * \param x : bigint (can't be NULL)
 * \return : ErrorMessage
 */
ErrorMessage big_show_dec(bigint* x)
{
	if (x == NULL)
		return FAIL_NULL;
	int wordlen = x->wordlen;
	bigint* tmpX = NULL;
	bigint* ten = NULL;
	bigint* digit = NULL;
	big_assign(&tmpX, x);

	if (x->sign == NEGATIVE)
	{
		printf("-");
		big_flip_sign(&tmpX);
	}

	big_new(&ten, NON_NEGATIVE, 1);
	ten->a[0] = 10;

	//output
	//digits =  ceil(n * log2) + 1   ( 0.3 < log2 < 0.4)
	int digits = (int)ceil((double)x->wordlen * WORD_UNIT * 0.4) + 1;
	int ptr = 0;
	char* output = (char*)calloc(digits, sizeof(char));
	if (output == NULL)
		return FAIL_NULL;

	// while x != 0 , x /= 10, digit = x % 10, (output||digit)
	while (!big_is_zero(tmpX))
	{
		big_division(&tmpX, &digit, tmpX, ten);
		output[ptr] = digit2char(digit->a[0]);
		ptr++;
	}
	output[ptr] = '\0';

	//reverse
	reverseStr(output);

	//output
	printf("%s\n", output);

	free(output);
	big_delete(&ten);
	big_delete(&digit);
	big_delete(&tmpX);
	return SUCCESS;
}
/**
 * show bigint by binary.
 *
 * \param x : bigint (can't be NULL)
 * \return : ErrorMessage
 */
ErrorMessage big_show_bin(bigint* x) {
	if (x == NULL)
		return FAIL_NULL;
	int wordlen = x->wordlen;
	if (x->sign == NEGATIVE)
		printf("-");
	printf("0b");
	for (int i = wordlen - 1; i >= 0; i--)
	{
		word current = x->a[i];
		int length = WORD_UNIT;
		while (length--)
		{
			printf("%d", (int)(current >> length) & 0x1);
		}
	}
	printf("\n");
	return SUCCESS;
}
// static function
static ErrorMessage big_refine(bigint* x) {
	if (x == NULL)
		return FAIL_NULL;
	int newWordlen = x->wordlen;
	while (newWordlen > 1)
	{
		if (x->a[newWordlen - 1] != 0)
			break;
		newWordlen--;
	}
	if (x->wordlen != newWordlen)
	{
		x->wordlen = newWordlen;
		word* reallocWords = (word*)realloc(x->a, sizeof(word) * newWordlen);
		if (reallocWords == NULL)
			return FAIL_OUT_OF_MEMORY;
		else
			x->a = reallocWords;
	}
	if ((x->wordlen == 1) && (x->a == 0x0))
		x->sign = NON_NEGATIVE;
	return SUCCESS;
}
/**
 * assign a bigint to another bigint.
 *
 * \param dst : address of bigint to be assigned (can be NULL)
 * \param src : bigint to assign (can't be NULL)
 * \return : ErrorMessage
 */
ErrorMessage big_assign(bigint** dst, bigint* src) {
	if (*dst == src)return SUCCESS;
	big_new(dst, src->sign, src->wordlen);
	memcpy((*dst)->a, src->a, sizeof(word) * (src->wordlen));
	return SUCCESS;
}
/**
 * generate a random bigint.
 *
 * \param x : address of bigint (can be NULL)
 * \param sign : sign of bigint (NON_NEGATIVE = 0, NEGATIVE = 1)
 * \param wordlen : word length of bigint (int type)
 * \return : ErrorMessage
 */
ErrorMessage big_gen_rand(bigint** x, int sign, int wordlen)
{
	big_new(x, sign, wordlen);
	do {
		array_rand((*x)->a, wordlen);
	} while ((*x)->a[wordlen - 1] == 0);
	return SUCCESS;
}
/**
 * return word length of bigint.
 *
 * \param x : bigint (can't be NULL)
 * \return : word length of bigint
 */
int big_get_wordlen(bigint* x) {
	if (x == NULL)
		return FAIL_NULL;
	return (x->wordlen);
}
/**
 * return bit length of bigint.
 *
 * \param x : bigint (can't be NULL)
 * \return : big length of bigint
 */
int big_get_bitlen(bigint* x) {
	if (x == NULL)
		return FAIL_NULL;
	word MSW = x->a[x->wordlen - 1];
	int bitLen = 0;
#if WORD_UNIT == 8
	bitLen = (x->wordlen - 1) << 3;
#elif WORD_UNIT == 32
	bitLen = (x->wordlen - 1) << 5;
#elif WORD_UNIT == 64
	bitLen = (x->wordlen - 1) << 6;
#endif
	while (MSW >>= 1)
		bitLen++;
	bitLen++;
	return bitLen;
}
/**
 * return a certain bit of bigint.
 *
 * \param x : bigint (can't be NULL)
 * \param bitindex : position of certain bit (int type)
 * \return : certain bit of bigint
 */
int big_get_bit(bigint* x, int bitindex) {
	if (x == NULL)
		return FAIL_NULL;
#if WORD_UNIT == 8
	int wordIndex = bitindex >> 3;
#elif WORD_UNIT == 32
	int wordIndex = bitindex >> 5;
#elif WORD_UNIT == 64
	int wordIndex = bitindex >> 6;
#endif
	if (wordIndex >= x->wordlen)
		return FAIL_OUT_OF_RANGE;

	int bitInWord = bitindex - wordIndex * WORD_UNIT;
	word currentWord = x->a[wordIndex];
	return (currentWord >> bitInWord) & 0x1;
}
/**
 * return sign of bigint.
 *
 * \param x : bigint (can't be NULL)
 * \return : sign of bigint (NON_NEGATIVE = 0, NEGATIVE = 1)
 */
int big_get_sign(bigint* x) {
	if (x == NULL)
		return FAIL_NULL;
	return (x->sign);
}
/**
 * flip sign of bigint.
 *
 * \param x : bigint (can't be NULL)
 * \return : ErrorMessage
 */
ErrorMessage big_flip_sign(bigint** x) {
	if (x == NULL)
		return FAIL_NULL;
	if (big_is_zero(*x))
		return SUCCESS;
	(*x)->sign = !((*x)->sign);
	return SUCCESS;
}
/**
 * set the bigint to 0.
 *
 * \param x : address of bigint (can be NULL)
 * \return : ErrorMessage
 */
ErrorMessage big_set_zero(bigint** x) {
	big_new(x, NON_NEGATIVE, 1);
	(*x)->a[0] = 0x0;
	return SUCCESS;
}
/**
 * set the bigint to 1.
 *
 * \param x : address of bigint (can be NULL)
 * \return : ErrorMessage
 */
ErrorMessage big_set_one(bigint** x) {
	big_new(x, NON_NEGATIVE, 1);
	(*x)->a[0] = 0x1;
	return SUCCESS;
}
/**
 * return if the bigint is 0 or not.
 *
 * \param x : bigint (can't be NULL)
 * \return : if the bigint is 0 or not
 */
bool big_is_zero(bigint* x) {
	if (x == NULL)
		return FAIL_NULL;
	return (x->sign == NON_NEGATIVE) && (x->wordlen == 1) && (x->a[0] == 0x0);
}
/**
 * return if the bigint is 1 or not.
 *
 * \param x : bigint (can't be NULL)
 * \return : if the bigint is 1 or not
 */
bool big_is_one(bigint* x) {
	if (x == NULL)
		return FAIL_NULL;
	return (x->sign == NON_NEGATIVE) && (x->wordlen == 1) && (x->a[0] == 0x1);
}
/**
 * return if the bigint is -1 or not.
 *
 * \param x : bigint (can't be NULL)
 * \return : if the bigint is -1 or not
 */
bool big_is_minus_one(bigint* x)
{
	if (x == NULL)
		return FAIL_NULL;
	return (x->sign == NEGATIVE) && (x->wordlen == 1) && (x->a[0] == 0x1);
}
/**
 * return if the bigint is odd or not.
 *
 * \param x : bigint (can't be NULL)
 * \return : if the bigint is odd or not
 */
bool big_is_odd(bigint* x)
{
	if (x == NULL)
		return FAIL_NULL;
	return ((x->a[0]) & 0x1);
}
/**
 * return if the bigint is even or not.
 *
 * \param x : bigint (can't be NULL)
 * \return : if the bigint is even or not
 */
bool big_is_even(bigint* x)
{
	if (x == NULL)
		return FAIL_NULL;
	return !((x->a[0]) & 0x1);
}
/**
 * return if gcd(x, y) = 1.
 * 
 * \param x : bigint (can't be NULL) (x > 1)
 * \param y : bigint (can't be NULL) (x > 1)
 * \return : if gcd(x, y) = 1
 */
bool big_is_relatively_prime(bigint* x, bigint* y)
{
	if (x == NULL || y == NULL)return FAIL_NULL;
	if (x->sign == NEGATIVE || y->sign == NEGATIVE) return FAIL_INVALID_INPUT;
	if (big_is_zero(x) || big_is_one(x) || big_is_zero(y) || big_is_one(y)) return FAIL_INVALID_INPUT;
	bigint* d = NULL;
	big_gcd(&d, x, y);
	bool ret = big_is_one(d);
	big_delete(&d);
	return ret;
}
/**
 * return the comparison between two bigints.
 *
 * \param x : one bigint (can't be NULL)
 * \param y : the other bigint (can't be NULL)
 * \return : comparison of two bigints (1 = x is bigger, 0 = two numbers are equal, -1 = y is bigger)
 */
int big_compare(bigint* x, bigint* y) {
	if (x == NULL || y == NULL)
		return FAIL_NULL;
	int xSign = x->sign;
	int ySign = y->sign;
	// 1. if both signs are same -> compareABS
	// 2. if both signs are different -> NON_NEGATIVE is bigger
	if (xSign == ySign)
	{
		//both are non-negative
		if (xSign == NON_NEGATIVE)
		{
			if (big_is_zero(x))
			{
				if (big_is_zero(y))
					return EQUAL;
				else
					return SMALLER;

			}
			else
			{
				if (big_is_zero(y))
					return BIGGER;
				else
				{
					return (big_compareABS(x, y));
				}
			}
		}
		//both are negative
		else
		{
			return -big_compareABS(x, y);
		}
	}
	else
	{
		//the one is non-negative, the other is negative.
		if (xSign == NON_NEGATIVE)
			return BIGGER;
		else
			return SMALLER;
	}
}

// static function
static int big_compareABS(bigint* x, bigint* y)
{
	int xWordlen = x->wordlen;
	int yWordlen = y->wordlen;
	// 1. if both word lengths are same -> compare each word 
	// 1. if both word lengths are different -> the longer integer is bigger
	if (xWordlen < yWordlen)
		return SMALLER;
	else if (xWordlen > yWordlen)
		return BIGGER;
	else
	{
		word* xWords = x->a;
		word* yWords = y->a;
		for (int i = xWordlen - 1; i >= 0; i--)
		{
			if (xWords[i] > yWords[i])
				return BIGGER;
			else if (xWords[i] < yWords[i])
				return SMALLER;
		}
		return EQUAL;
	}
}
/**
 * bit left shift operation.
 * <<
 * \param dst : address of bigint to be assigned (can be NULL)
 * \param src : bigint for shifting (can't be NULL)
 * \param count : shifting bit
 * \return : ErrorMessage
 */
ErrorMessage big_bit_left_shift(bigint** dst, bigint* src, int count) {
	if (src == NULL)
		return FAIL_NULL;
	if (count == 0)
	{
		big_assign(dst, src);
		return SUCCESS;
	}
	// count = quotient * WORD_UNIT + remainder
	int remainder = count % WORD_UNIT;
	int quotient = (count - remainder);
#if WORD_UNIT==8
	quotient >>= 3;
#elif WORD_UNIT==32
	quotient >>= 5;
#elif WORD_UNIT==64
	quotient >>= 6;
#endif

	int newWordlen = src->wordlen + quotient;
	// if count = quotient * WORD_UNIT -> big_word_shift(quotient)
	if (remainder == 0)
	{
		big_word_left_shift(dst, src, quotient);
		return SUCCESS;
	}
	else
		newWordlen += 1;

	bigint* tmp = NULL;
	big_new(&tmp, src->sign, newWordlen);
	for (int i = 0; i < newWordlen; i++)
	{
		if (i < quotient)
			tmp->a[i] = 0;
		else if (i == quotient)
			tmp->a[i] = ((src->a[0]) << remainder);
		else if (i == newWordlen - 1)
			tmp->a[i] = (src->a[src->wordlen - 1]) >> (WORD_UNIT - remainder);
		else
			tmp->a[i] = ((src->a[i - quotient]) << remainder) | ((src->a[i - quotient - 1]) >> (WORD_UNIT - remainder));
	}

	big_refine(tmp);
	big_assign(dst, tmp);
	big_delete(&tmp);

	return SUCCESS;
}
/**
 * bit right shift operation.
 * >>
 * \param dst : address of bigint to be assigned (can be NULL)
 * \param src : bigint for shifting (can't be NULL)
 * \param count : shifting bit
 * \return : ErrorMessage
 */
ErrorMessage big_bit_right_shift(bigint** dst, bigint* src, int count) {
	if (src == NULL)
		return FAIL_NULL;
	if (big_get_bitlen(src) <= count)
	{
		big_set_zero(dst);
		return SUCCESS;
	}
	if (count == 0)
	{
		big_assign(dst, src);
		return SUCCESS;
	}
	
	// count = quotient * WORD_UNIT + remainder
	int remainder = count % WORD_UNIT;
	int quotient = (count - remainder);
#if WORD_UNIT==8
	quotient >>= 3;
#elif WORD_UNIT==32
	quotient >>= 5;
#elif WORD_UNIT==64
	quotient >>= 6;
#endif

	int newWordlen = src->wordlen - quotient;
	// if count = quotient * WORD_UNIT -> big_word_shift(quotient)
	if (remainder == 0)
	{
		big_word_right_shift(dst, src, quotient);
		return SUCCESS;
	}

	bigint* tmp = NULL;
	big_new(&tmp, src->sign, newWordlen);

	for (int i = 0; i < newWordlen; i++)
	{
		if (i == newWordlen - 1)
			tmp->a[i] = ((src->a[src->wordlen - 1]) >> remainder);
		else
			tmp->a[i] = ((src->a[i + quotient + 1]) << (WORD_UNIT - remainder) | ((src->a[i + quotient]) >> remainder));
	}

	big_refine(tmp);
	big_assign(dst, tmp);
	big_delete(&tmp);

	return SUCCESS;
}
/**
 * bit reduction operation.
 * mod 2^n
 * \param dst : address of bigint to be assigned (can be NULL)
 * \param src : bigint for shifting (can't be NULL)
 * \param count : reduction bit
 * \return : ErrorMessage
 */
ErrorMessage big_bit_reduction(bigint** dst, bigint* src, int count)
{
	if (src == NULL)
		return FAIL_NULL;
	if (big_get_bitlen(src) <= count)
	{
		big_assign(dst, src);
		return SUCCESS;
	}
	if (count == 0)
	{
		big_set_zero(dst);
		return SUCCESS;
	}
	int remainder = count % WORD_UNIT;
	int quotient = (count - remainder);
#if WORD_UNIT==8
	quotient >>= 3;
#elif WORD_UNIT==32
	quotient >>= 5;
#elif WORD_UNIT==64
	quotient >>= 6;
#endif
	// assign
	int newWordlen = quotient;
	if (remainder == 0)
	{
		big_word_reduction(dst, src, quotient);
	}
	else
		newWordlen++;
	bigint* tmp = NULL;
	big_new(&tmp, src->sign, newWordlen);
	for (int i = 0; i < newWordlen; i++)
	{
		if (i == newWordlen - 1)
		{
			word bitMask = 0x1;
			bitMask <<= remainder;
			bitMask -= 1;
			tmp->a[i] = (src->a[i]) & (bitMask);
		}
		else
			tmp->a[i] = src->a[i];
	}

	big_refine(tmp);
	big_assign(dst, tmp);
	big_delete(&tmp);
	return SUCCESS;
}
/**
 * word left shift operation.
 * <<
 * \param dst : address of bigint to be assigned (can be NULL)
 * \param src : bigint for shifting (can't be NULL)
 * \param count : shifting word
 * \return : ErrorMessage
 */
ErrorMessage big_word_left_shift(bigint** dst, bigint* src, int count)
{
	if (src == NULL)
		return FAIL_NULL;
	if (count == 0)
	{
		big_assign(dst, src);
		return SUCCESS;
	}
	// alloc
	bigint* tmp = NULL;
	int newWordlen = src->wordlen + count;
	big_new(&tmp, src->sign, newWordlen);

	// shift
	for (int i = 0; i < newWordlen; i++)
	{
		if (i < count)
			tmp->a[i] = 0;
		else
			tmp->a[i] = src->a[i - count];
	}

	big_refine(tmp);
	big_assign(dst, tmp);
	big_delete(&tmp);

	return SUCCESS;
}
/**
 * word right shift operation.
 * >>
 * \param dst : address of bigint to be assigned (can be NULL)
 * \param src : bigint for shifting (can't be NULL)
 * \param count : shifting word
 * \return : ErrorMessage
 */
ErrorMessage big_word_right_shift(bigint** dst, bigint* src, int count)
{
	if (src == NULL)
		return FAIL_NULL;
	if (src->wordlen <= count)
	{
		big_set_zero(dst);
		return SUCCESS;
	}
	if (count == 0)
	{
		big_assign(dst, src);
		return SUCCESS;
	}
	// alloc
	bigint* tmp = NULL;
	int newWordlen = src->wordlen - count;
	big_new(&tmp, src->sign, newWordlen);
	// shift
	for (int i = 0; i < newWordlen; i++)
	{
		tmp->a[i] = src->a[i + count];
	}
	big_refine(tmp);
	big_assign(dst, tmp);
	big_delete(&tmp);

	return SUCCESS;
}
/**
 * word reduction operation.
 * mod 2^nW
 * \param dst : address of bigint to be assigned (can be NULL)
 * \param src : bigint for shifting (can't be NULL)
 * \param count : reduction word
 * \return : ErrorMessage
 */
ErrorMessage big_word_reduction(bigint** dst, bigint* src, int count)
{
	if (src == NULL)
		return FAIL_NULL;
	if (src->wordlen <= count)
	{
		big_assign(dst, src);
		return SUCCESS;
	}
	if (count == 0)
	{
		big_set_zero(dst);
		return SUCCESS;
	}
	// alloc
	bigint* tmp = NULL;
	int newWordlen = count;
	big_new(&tmp, src->sign, newWordlen);
	// reduction
	for (int i = 0; i < newWordlen; i++)
	{
		tmp->a[i] = src->a[i];
	}
	// assign
	big_refine(tmp);
	big_assign(dst, tmp);
	big_delete(&tmp);

	return SUCCESS;
}
/**
 * addition x + y = z.
 *
 * \param z : address of bigint z (can be NULL)
 * \param x : one bigint (can't be NULL)
 * \param y : another bigint (can't be NULL)
 * \return : ErrorMessage
 */
ErrorMessage big_addition(bigint** z, bigint* x, bigint* y)
{
	if (x == NULL || y == NULL)
		return FAIL_NULL;
	// alloc
	bigint* tmp = NULL;

	// x = 0 -> x + y = y
	if (big_is_zero(x))
		big_assign(&tmp, y);
	// y = 0 -> x + y = x
	else if (big_is_zero(y))
		big_assign(&tmp, x);
	else
	{
		int xSign = x->sign;
		int ySign = y->sign;
		// x < 0 && y > 0 -> x + y = y - absx
		if ((xSign == NEGATIVE) && (ySign == NON_NEGATIVE))
		{
			bigint* absX = NULL;
			big_assign(&absX, x);
			big_flip_sign(&absX);
			big_subtraction(&tmp, y, absX);
			big_delete(&absX);
		}
		// x > 0 && y < 0 -> x + y = x - absy
		else if ((xSign == NON_NEGATIVE) && (ySign == NEGATIVE))
		{
			bigint* absY = NULL;
			big_assign(&absY, y);
			big_flip_sign(&absY);
			big_subtraction(&tmp, x, absY);
			big_delete(&absY);
		}
		else
		{
			big_additionABS(&tmp, x, y);
			// x < 0 && y < 0 -> -additionAbs(x,y)
			if (xSign == NEGATIVE)
				tmp->sign = NEGATIVE;
			// x > 0 && y > 0 -> additionAbs(x,y)
			else
				tmp->sign = NON_NEGATIVE;
		}

	}
	big_assign(z, tmp);
	big_delete(&tmp);
	return SUCCESS;
}
// static function
static ErrorMessage big_additionABS(bigint** z, bigint* x, bigint* y)
{
	word* xWords = x->a;
	word* yWords = y->a;
	int xWordlen = x->wordlen;
	int yWordlen = y->wordlen;

	int newWordlen = MAX(xWordlen, yWordlen);
	newWordlen += 1;

	big_new(z, NON_NEGATIVE, newWordlen);

	word carryFromDown = 0;
	word carryToUp = 0;
	for (int i = 0; i < newWordlen; i++)
	{
		// A , B
		word xWord = 0;
		word yWord = 0;
		//if A or B do not have value -> A = 0, B = 0
		if (i < xWordlen)
			xWord = xWords[i];
		if (i < yWordlen)
			yWord = yWords[i];

		// if A + B < A   -> carryUp = 1
		word sum = xWord + yWord;
		if (sum < xWord)
			carryToUp = 1;

		// if sum + carryFromDown < carryFromDown -> carryUp = 1
		sum += carryFromDown;
		if (sum < carryFromDown)
			carryToUp = 1;

		// assign
		(*z)->a[i] = sum;

		carryFromDown = carryToUp;
		carryToUp = 0;
	}
	big_refine(*z);

	return SUCCESS;
}
/**
 * subtraction x - y = z.
 *
 * \param z : address of bigint z (can be NULL)
 * \param x : one bigint (can't be NULL)
 * \param y : another bigint (can't be NULL)
 * \return : ErrorMessage
 */
ErrorMessage big_subtraction(bigint** z, bigint* x, bigint* y)
{
	if (x == NULL || y == NULL)
		return FAIL_NULL;
	//alloc
	bigint* tmp = NULL;

	int comp = big_compare(x, y);
	// x = 0 -> x - y = -y
	if (big_is_zero(x))
	{
		big_assign(&tmp, y);
		big_flip_sign(&tmp);
	}
	// y = 0 -> x - y = x
	else if (big_is_zero(y))
	{
		big_assign(&tmp, x);
	}
	// x = y -> x - y = 0
	else if (comp == EQUAL)
	{
		big_set_zero(&tmp);
	}
	else
	{
		int xSign = x->sign;
		int ySign = y->sign;

		// x < 0 && y > 0 -> x - y = - (absx + y)
		if ((xSign == NEGATIVE) && (ySign == NON_NEGATIVE))
		{
			big_additionABS(&tmp, x, y);
			big_flip_sign(&tmp);
		}
		// x > 0 && y < 0 -> x - y = x + absy
		else if ((xSign == NON_NEGATIVE) && (ySign == NEGATIVE))
		{
			big_additionABS(&tmp, x, y);
		}
		else if ((xSign == NON_NEGATIVE) && (ySign == NON_NEGATIVE))
		{
			// x > y > 0 -> x - y = subAbs(x,y)
			if (comp == BIGGER)
				big_subtractionABS(&tmp, x, y);
			// y > x > 0 -> x - y = -subAbs(y,x)
			else
			{
				big_subtractionABS(&tmp, y, x);
				big_flip_sign(&tmp);
			}
		}
		else
		{
			// 0 > x > y -> x - y = (absy) - (absx) = subAbs(y,x)
			if (comp == BIGGER)
				big_subtractionABS(&tmp, y, x);
			// 0 > y > x -> x - y = -(-x) + (-y) = - (absx - absy) = -subAbs(x,y)
			else
			{
				big_subtractionABS(&tmp, x, y);
				big_flip_sign(&tmp);
			}
		}

	}
	big_assign(z, tmp);
	big_delete(&tmp);
	return SUCCESS;
}
// static function
static ErrorMessage big_subtractionABS(bigint** z, bigint* x, bigint* y)
{
	word* xWords = x->a;
	word* yWords = y->a;
	int xWordlen = x->wordlen;
	int yWordlen = y->wordlen;
	int newWordlen = MAX(xWordlen, yWordlen);

	big_new(z, NON_NEGATIVE, newWordlen);

	word borrow = 0;
	for (int i = 0; i < newWordlen; i++)
	{
		// A , B
		word xWord = 0;
		word yWord = 0;
		//if A or B do not have value -> A = 0, B = 0
		if (i < xWordlen)
			xWord = xWords[i];
		if (i < yWordlen)
			yWord = yWords[i];

		word dif = xWord - borrow;
		if (xWord < borrow)
			borrow = 1;
		else
			borrow = 0;

		if (dif < yWord)
			borrow += 1;
		dif = dif - yWord;

		// assign
		(*z)->a[i] = dif;
	}
	big_refine(*z);

	return SUCCESS;
}
/**
 * multiplication x * y = z.
 *
 * \param z : address of bigint z (can be NULL)
 * \param x : one bigint (can't be NULL)
 * \param y : another bigint (can't be NULL)
 * \return : ErrorMessage
 */
ErrorMessage big_multiplication(bigint** z, bigint* x, bigint* y)
{
	if (x == NULL || y == NULL)
		return FAIL_NULL;
	// alloc
	bigint* tmp = NULL;

	// x = 0 or y = 0  -> x * y = 0
	if (big_is_zero(x) || big_is_zero(y))
		big_set_zero(&tmp);
	// x = 1 -> x * y = y
	else if (big_is_one(x))
		big_assign(&tmp, y);
	// y = 1 -> x * y = x
	else if (big_is_one(y))
		big_assign(&tmp, x);
	// x = -1 -> x * y = -y
	else if (big_is_minus_one(x))
	{
		big_assign(&tmp, y);
		big_flip_sign(&tmp);
	}
	// y = -1 -> x * y = -x
	else if (big_is_minus_one(y))
	{
		big_assign(&tmp, x);
		big_flip_sign(&tmp);
	}
	else if (big_compare(x, y) == EQUAL)
	{
		big_squaring(&tmp, x);
	}
	else
	{
		// tmp = |x| * |y|
		if (x->sign == NON_NEGATIVE)
		{
			if (y->sign == NON_NEGATIVE)
				big_multiplicationABS(&tmp, x, y);
			else
			{
				bigint* absY = NULL;
				big_assign(&absY, y);
				absY->sign = NON_NEGATIVE;
				big_multiplicationABS(&tmp, x, absY);
				big_delete(&absY);
			}
		}
		else
		{
			bigint* absX = NULL;
			big_assign(&absX, x);
			absX->sign = NON_NEGATIVE;
			if (y->sign == NON_NEGATIVE)
			{
				big_multiplicationABS(&tmp, absX, y);
			}
			else
			{
				bigint* absY = NULL;
				big_assign(&absY, y);
				absY->sign = NON_NEGATIVE;
				big_multiplicationABS(&tmp, absX, absY);
				big_delete(&absY);
			}
			big_delete(&absX);
		}
		// if x * y < 0 -> negative
		if (x->sign != y->sign)
			tmp->sign = NEGATIVE;
		// if x * y > 0 -> non_negative
		else
			tmp->sign = NON_NEGATIVE;
	}
	big_assign(z, tmp);
	big_delete(&tmp);
	return SUCCESS;

}
// static function
static ErrorMessage big_multiplicationABS(bigint** z, bigint* x, bigint* y)
{
	int newWordlen = x->wordlen + y->wordlen;
	big_new(z, NON_NEGATIVE, newWordlen);

#if karatsuba
	big_multiplicationKaratsuba(z, x, y);
#else
	big_multiplicationSchoolBook(z, x, y);
#endif

	big_refine(*z);
	return SUCCESS;
}
// static function
static ErrorMessage big_multiplicationSchoolBook(bigint** z, bigint* x, bigint* y)
{
	if (x == NULL || y == NULL)
		return FAIL_NULL;
	int xWordlen = x->wordlen;
	int yWordlen = y->wordlen;

	// alloc
	big_set_zero(z);

	// mul and add
	bigint* T0 = NULL;
	bigint* T1 = NULL;
	for (int i = 0; i < xWordlen; i++)
	{
		word currentWord = x->a[i];
		// T0 = (Ai * Bj) 's lower word
		// T1 = (Ai * B1) 's upper word
		big_new(&T0, NON_NEGATIVE, yWordlen + i + 2);
		big_new(&T1, NON_NEGATIVE, yWordlen + i + 2);
		for (int j = 0; j < yWordlen; j++)
		{
			word A = 0;
			word B = 0;
			
			wordMultiplication(&A, &B, currentWord, y->a[j]);

			T1->a[i + j + 1] = A;
			T0->a[i + j] = B;
		}
		big_addition(z, *z, T0);
		big_addition(z, *z, T1);
	}
	big_delete(&T0);
	big_delete(&T1);

	return SUCCESS;
}
// static function
static ErrorMessage big_multiplicationKaratsuba(bigint** z, bigint* x, bigint* y)
{
	if (x == NULL || y == NULL)
		return FAIL_NULL;

	int xWordlen = x->wordlen;
	int yWordlen = y->wordlen;
	if (xWordlen <= karaFlag || yWordlen <= karaFlag)
	{
		big_multiplicationSchoolBook(z, x, y);
	}
	else
	{
		// calculate shift bit
		int l = MAX(xWordlen, yWordlen);
		l++;
		l = l >> 1;
		// init
		bigint* A1 = NULL;
		bigint* A0 = NULL;
		bigint* B1 = NULL;
		bigint* B0 = NULL;
		bigint* T1 = NULL;
		bigint* T0 = NULL;
		bigint* S1 = NULL;
		bigint* S0 = NULL;
		bigint* R = NULL;

		// A1, A0, B1, B0
		big_word_right_shift(&A1, x, l);
		big_word_reduction(&A0, x, l);
		big_word_right_shift(&B1, y, l);
		big_word_reduction(&B0, y, l);

		// T1 = A1 * B1, T0 = A0 * B0
		big_multiplicationKaratsuba(&T1, A1, B1);
		big_multiplicationKaratsuba(&T0, A0, B0);

		// R = T1 << 2l + T0
		int rWordlen = T1->wordlen + 2 * l;
		big_new(&R, NON_NEGATIVE, rWordlen);
		for (int i = 0; i < rWordlen; i++)
		{
			if (i < 2 * l)
			{
				if (T0->wordlen > i)
					R->a[i] = T0->a[i];
			}
			else
			{
				R->a[i] = T1->a[i - 2 * l];
			}
		}

		// S1 = A0 - A1, S0 = B1 - B0
		big_subtraction(&S1, A0, A1);
		big_subtraction(&S0, B1, B0);

		// S = S1 * S0
		bool isNegative = (S1->sign != S0->sign);
		S0->sign = NON_NEGATIVE;
		S1->sign = NON_NEGATIVE;
		big_multiplicationKaratsuba(z, S0, S1);
		if (isNegative)
			(*z)->sign = NEGATIVE;

		// AB = R + (S + T1 + T0)<<l
		big_addition(z, *z, T1);
		big_addition(z, *z, T0);
		big_word_left_shift(z, *z, l);
		big_addition(z, *z, R);

		//delete
		big_delete(&A1);
		big_delete(&A0);
		big_delete(&B1);
		big_delete(&B0);
		big_delete(&T1);
		big_delete(&T0);
		big_delete(&S1);
		big_delete(&S0);
		big_delete(&R);
	}
	return SUCCESS;
}
/**
 * multiplication with constant x * y = z.
 *
 * \param z : address of bigint z (can be NULL)
 * \param x : bigint (can't be NULL)
 * \param y : word
 * \return : ErrorMessage
 */
ErrorMessage big_multiplicationConst(bigint** z, bigint* x, word y)
{
	if (x == NULL)
		return FAIL_NULL;
	word constant[1] = { y };
	//alloc
	bigint* tmp = NULL;
	big_set_by_array(&tmp, NON_NEGATIVE, constant, 1);

	big_multiplication(z, x, tmp);
	
	big_delete(&tmp);
	return SUCCESS;
}

/**
 * squaring z = x * x.
 *
 * \param z : address of bigint z (can be NULL)
 * \param x : bigint (can't be NULL)
 * \return : ErrorMessage
 */
ErrorMessage big_squaring(bigint** z, bigint* x)
{
	if (x == NULL)
		return FAIL_NULL;
	// alloc
	bigint* tmp = NULL;

	if (big_is_zero(x))
		big_set_zero(&tmp);
	else if (big_is_one(x) || big_is_minus_one(x))
		big_set_one(&tmp);
	else
	{
		bigint* absX = NULL;
		big_assign(&absX, x);
		absX->sign = NON_NEGATIVE;
		big_squaringABS(&tmp, absX);
		big_delete(&absX);
	}

	big_assign(z, tmp);
	big_delete(&tmp);
	return SUCCESS;
}
// static function
static ErrorMessage big_squaringABS(bigint** z, bigint* x)
{
	int newWordlen = (x->wordlen) << 1;

	big_new(z, NON_NEGATIVE, newWordlen);

#if karatsuba
	big_squaringKaratsuba(z, x);
#else
	big_squaringSchoolBook(z, x);
#endif

	big_refine(*z);
	return SUCCESS;
}
// static function
static ErrorMessage big_squaringSchoolBook(bigint** z, bigint* x)
{
	if (x == NULL)
		return FAIL_NULL;

	word* xWords = x->a;
	int xWordlen = x->wordlen;

	// alloc
	bigint* C1 = NULL;
	bigint* C2 = NULL;
	bigint* T0 = NULL;
	bigint* T1 = NULL;

	big_new(&C1, NON_NEGATIVE, 2 * xWordlen);
	big_set_zero(&C2);
	// mul and add
	for (int i = 0; i < xWordlen; i++)
	{
		// C1 = A^2 || A^2 || A^2 || A^2 || A^2 ...
		word A0 = 0, A1 = 0;
		wordSquaring(&A1, &A0, xWords[i]);

		C1->a[i * 2] = A0;
		C1->a[i * 2 + 1] = A1;

		// T0 = (Ai * Ai) 's lower word
		// T1 = (Ai * Ai) 's upper word
		big_new(&T0, NON_NEGATIVE, i + xWordlen + 2);
		big_new(&T1, NON_NEGATIVE, i + xWordlen + 2);

		for (int j = i + 1; j < xWordlen; j++)
		{
			// C2 = (AiAj) << w (i + j)
			word A = 0, B = 0;
			wordMultiplication(&A, &B, xWords[i], xWords[j]);
			T1->a[i + j + 1] = A;
			T0->a[i + j] = B;
		}
		// tmp = tmp + T
		big_addition(&C2, C2, T1);
		big_addition(&C2, C2, T0);
	}

	// C2 = C2 << 1
	big_bit_left_shift(&C2, C2, 1);

	big_additionABS(z, C1, C2);

	big_delete(&C1);
	big_delete(&C2);
	big_delete(&T0);
	big_delete(&T1);
	return SUCCESS;
}
// static function
static ErrorMessage big_squaringKaratsuba(bigint** z, bigint* x)
{
	if (x == NULL)
		return FAIL_NULL;
	int xWordlen = x->wordlen;
	if (xWordlen <= karaFlag)
	{
		big_squaringSchoolBook(z, x);
	}
	else
	{
		// calculate shift bit
		int l = (xWordlen + 1) >> 1;

		//alloc
		bigint* A1 = NULL;
		bigint* A0 = NULL;
		bigint* T1 = NULL;
		bigint* T0 = NULL;
		bigint* R = NULL;
		bigint* S = NULL;

		// A1, A0
		big_word_right_shift(&A1, x, l);
		big_word_reduction(&A0, x, l);

		// T1, T0
		big_squaringKaratsuba(&T1, A1);
		big_squaringKaratsuba(&T0, A0);

		// R = T1 << 2l + T0
		int rWordlen = T1->wordlen + 2 * l;
		big_new(&R, NON_NEGATIVE, rWordlen);
		for (int i = 0; i < rWordlen; i++)
		{
			if (i < 2 * l)
			{
				if (T0->wordlen > i)
					R->a[i] = T0->a[i];
			}
			else
			{
				R->a[i] = T1->a[i - 2 * l];
			}
		}

		// S = (A1 * A0) << (lw + 1)
		big_multiplicationKaratsuba(&S, A1, A0);
		big_word_left_shift(&S, S, l);
		big_bit_left_shift(&S, S, 1);

		// R = R + S
		big_additionABS(z, R, S);

		//delete
		big_delete(&A1);
		big_delete(&A0);
		big_delete(&T1);
		big_delete(&T0);
		big_delete(&S);
		big_delete(&R);
	}
	return SUCCESS;
}

/**
 * division x = y * q + r (0 <= r < y).
 *
 * \param q : address of bigint q (can be NULL)
 * \param r : address of bigint r (can be NULL)
 * \param x : bigint x (can't be NULL)
 * \param y : bigint y (can't be NULL) (y > 0)
 * \return : ErrorMessage
 */
ErrorMessage big_division(bigint** q, bigint** r, bigint* x, bigint* y)
{
	if (x == NULL || y == NULL)
		return FAIL_NULL;
	// don't define y <= 0 
	if (y->sign == NEGATIVE || big_is_zero(y))
		return FAIL_INVALID_DIVISOR;
	// alloc
	bigint* tmpQ = NULL;
	bigint* tmpR = NULL;

	// if x < y -> x / y = 0 ... x
	if (big_compare(x, y) == SMALLER)
	{
		big_set_zero(&tmpQ);
		big_assign(&tmpR, x);
	}
	// if x == y -> x / y = 1 ... 0
	else if (big_compare(x, y) == EQUAL)
	{
		big_set_one(&tmpQ);
		big_set_zero(&tmpR);
	}
	// if x >= 0 -> divisionABS(x, y)
	else if (x->sign == NON_NEGATIVE)
	{
		big_divisionABS(&tmpQ, &tmpR, x, y);
	}
	// if x < 0 -> divisionABS(-x, y) -> r = b - r', q = -q' - 1
	else
	{
		bigint* absX = NULL;
		bigint* one = NULL;
		big_assign(&absX, x);
		absX->sign = NON_NEGATIVE;

		// calculate Q' and R'
		big_divisionABS(&tmpQ, &tmpR, absX, y);

		// calculate Q and R,  Q = -Q' - 1, R = B - R'
		big_set_one(&one);
		big_addition(&tmpQ, tmpQ, one);
		big_flip_sign(&tmpQ);
		big_subtraction(&tmpR, y, tmpR);
		big_delete(&absX);
		big_delete(&one);
	}
	big_assign(q, tmpQ);
	big_assign(r, tmpR);
	big_delete(&tmpQ);
	big_delete(&tmpR);
	return SUCCESS;
}
// static function
static ErrorMessage big_divisionABS(bigint** q, bigint** r, bigint* x, bigint* y)
{
	// alloc
	int qWordlen = x->wordlen;
	big_new(q, NON_NEGATIVE, qWordlen);
	big_set_zero(r);

	// main logic : multi-precision long division
	for (int i = x->wordlen - 1; i > -1; i--)
	{
		// R = RW + Ai
		big_word_left_shift(r, *r, 1);
		(*r)->a[0] = x->a[i];

		// (Qi, R) <- DIVC(R, B)
		word Qi = 0;
		big_divisionCore(&Qi, r, *r, y);
		(*q)->a[i] = Qi;
	}

	big_refine(*q);
	big_refine(*r);
	return SUCCESS;
}
// static function
static ErrorMessage big_divisionCore(word* q, bigint** r, bigint* x, bigint* y)
{
	if (x == NULL || y == NULL)
		return FAIL_NULL;

	if (big_compare(x, y) == SMALLER)
	{
		*q = 0;
		big_assign(r, x);
	}
	else
	{
		//compute k such that 2^w-1 <= 2^k * Bm < 2^w 
		int k = 0;
		word MSW = y->a[y->wordlen - 1];
		while (MSW >>= 1)
			k++;
		k = WORD_UNIT - k - 1;
		bigint* shiftedA = NULL;
		bigint* shiftedB = NULL;

		// shift A and B
		big_bit_left_shift(&shiftedA, x, k);
		big_bit_left_shift(&shiftedB, y, k);
		
		// compute q' and r'
		big_divisionCoreCore(q, r, shiftedA, shiftedB);

		// q = q', r = r' >> k
		big_bit_right_shift(r, *r, k);

		big_delete(&shiftedA);
		big_delete(&shiftedB);
	}

	return SUCCESS;
}
// static function
static ErrorMessage big_divisionCoreCore(word* q, bigint** r, bigint* x, bigint* y)
{
	if (x == NULL || y == NULL)
		return FAIL_NULL;

	int xWordlen = x->wordlen;
	int yWordlen = y->wordlen;

	if (xWordlen == yWordlen)
	{
		*q = (x->a[yWordlen - 1] / y->a[yWordlen - 1]);
	}
	else
	{
		if (x->a[yWordlen] == y->a[yWordlen - 1])
			*q = WORD_MASK;
		else
		{
			wordLongDivision(q, x->a[yWordlen], x->a[yWordlen - 1], y->a[yWordlen - 1]);
		}
	}
	// R = A - BQ
	bigint* BQ = NULL;
	big_multiplicationConst(&BQ, y, *q);
	big_subtraction(r, x, BQ);

	// while R < 0 
	while ((*r)->sign == NEGATIVE)
	{
		(*q) -= 1;
		big_addition(r, *r, y);
	}
	big_delete(&BQ);
	return SUCCESS;
}

/**
 * modular x mod y = z.
 *
 * \param z : address of bigint z (can be NULL)
 * \param x : bigint x (can't be NULL)
 * \param y : bigint y (can't be NULL) (y > 0)
 * \return : ErrorMessage
 */
ErrorMessage big_mod(bigint** z, bigint* x, bigint* y)
{
	if (x == NULL || y == NULL)
		return FAIL_NULL;
	// don't define y <= 0 
	if (y->sign == NEGATIVE || big_is_zero(y))
		return FAIL_INVALID_DIVISOR;
	// alloc
	bigint* tmpQ = NULL;
	bigint* tmpR = NULL;

	if (big_compare(x, y) == SMALLER)
	{
		big_assign(&tmpR, x);
	}
	else if (big_compare(x, y) == EQUAL)
	{
		big_set_zero(&tmpR);
	}
	else if (x->sign == NON_NEGATIVE)
	{
		big_divisionABS(&tmpQ, &tmpR, x, y);
	}
	else
	{
		bigint* absX = NULL;
		big_assign(&absX, x);
		absX->sign = NON_NEGATIVE;

		// calculate Q' and R'
		big_divisionABS(&tmpQ, &tmpR, absX, y);

		// calculate Q and R,  Q = -Q' - 1, R = B - R'
		big_subtraction(&tmpR, y, tmpR);
		big_delete(&absX);
	}
	big_assign(z, tmpR);
	big_delete(&tmpQ);
	big_delete(&tmpR);
	return SUCCESS;
}

/**
 * modular inverse x * z = 1 (mod y).
 *
 * \param z : address of bigint z (can be NULL)
 * \param x : bigint x (can't be NULL)
 * \param y : bigint y (can't be NULL) (y > 0) (y is odd prime)
 * \return : ErrorMessage
 */
ErrorMessage big_mod_inverse(bigint** z, bigint* x, bigint* y)
{
	if (x == NULL || y == NULL)
		return FAIL_NULL;
	// don't define y <= 0 
	if (y->sign == NEGATIVE || big_is_zero(y))
		return FAIL_INVALID_DIVISOR;
	bigint* d = NULL;
	bigint* u = NULL;
	bigint* v = NULL;
	big_xgcd(&d, &u, &v, x, y);
	if (!big_is_one(d))
		return FAIL_INVALID_INPUT;

	if (u->sign == NEGATIVE)
		big_addition(&u, u, y);

	big_mod(z, u, y);

	big_delete(&d);
	big_delete(&u);
	big_delete(&v);
	return SUCCESS;
}

/**
 * modular exponentiation x ^ n mod y = z.
 *
 * \param z : address of bigint z (can be NULL)
 * \param x : bigint x (can't be NULL)
 * \param n : bigint n (can't be NULL) (n >= 0)
 * \param y : bigint y (can't be NULL) (y > 0)
 * \return : ErrorMessage
 */
ErrorMessage big_mod_exp(bigint** z, bigint* x, bigint* n, bigint* y)
{
	if (x == NULL || y == NULL)
		return FAIL_NULL;
	// don't define y <= 0 
	if (y->sign == NEGATIVE || big_is_zero(y))
		return FAIL_INVALID_DIVISOR;
	if (n->sign == NEGATIVE)
		return FAIL_INVALID_EXPONENT;
	// alloc
	bigint* tmp = NULL;

	if (big_is_zero(x))
	{
		big_set_zero(&tmp);
	}
	else if (x->sign == NON_NEGATIVE)
	{
		big_mod_expABS(&tmp, x, n, y);
	}
	else
	{
		bigint* absX = NULL;
		big_assign(&absX, x);
		absX->sign = NON_NEGATIVE;
		big_mod_expABS(&tmp, absX, n, y);
		if (big_is_odd(n))
			big_subtraction(&tmp, y, tmp);
		big_delete(&absX);
	}

	big_assign(z, tmp);
	big_delete(&tmp);

	return SUCCESS;
}
// static function
static ErrorMessage big_mod_expABS(bigint** z, bigint* x, bigint* n, bigint* y)
{
	// x = x mod y
	big_mod(z, *z, y);

	// choose main logic 

#if EXPMODMODE == 1
	big_mod_expL2R(z, x, n, y);
#elif EXPMODMODE == 2
	big_mod_expR2L(z, x, n, y);
#elif EXPMODMODE == 3
	big_mod_expMS(z, x, n, y);
#endif

	big_refine(*z);
	return SUCCESS;
}
// static function
static ErrorMessage big_mod_expL2R(bigint** z, bigint* x, bigint* n, bigint* y)
{
	int nWordlen = n->wordlen;
	// t = 1
	big_set_one(z);

	// mul and squaring loop
	for (int i = nWordlen - 1; i > -1; i--)
	{
		word currentWord = n->a[i];
		for (int j = WORD_UNIT - 1; j > -1; j--)
		{
			// t = t ^ 2
			big_squaring(z, *z);
			// mod
			big_mod(z, *z, y);

			if ((currentWord >> j) & 0x1)
			{
				big_multiplication(z, *z, x);
				// mod
				big_mod(z, *z, y);
			}
		}
	}
	return SUCCESS;
}
// static function
static ErrorMessage big_mod_expR2L(bigint** z, bigint* x, bigint* n, bigint* y)
{
	bigint* t1 = NULL;
	int nWordlen = n->wordlen;
	// t0 = 1, t1 = x
	big_set_one(z);
	big_assign(&t1, x);

	// mul and squaring loop
	for (int i = 0; i < nWordlen; i++)
	{
		word currentWord = n->a[i];
		for (int j = 0; j < WORD_UNIT; j++)
		{
			// t0 = t0 * t1 ^ bit
			if (currentWord & 0x1)
			{
				big_multiplication(z, *z, t1);
				// mod
				big_mod(z, *z, y);
			}
			currentWord >>= 1;
			// t1 = t1 ^ 2
			big_squaring(&t1, t1);
			//mod
			big_mod(&t1, t1, y);
		}
	}

	big_delete(&t1);
	return SUCCESS;
}
// static function
static ErrorMessage big_mod_expMS(bigint** z, bigint* x, bigint* n, bigint* y)
{
	bigint* t1 = NULL;
	int nWordlen = n->wordlen;
	// t0 = 1, t1 = x
	big_set_one(z);
	big_assign(&t1, x);

	// mul and squaring loop
	for (int i = nWordlen - 1; i > -1; i--)
	{
		word currentWord = n->a[i];
		for (int j = WORD_UNIT - 1; j > -1; j--)
		{
			// if ni = 0 -> t1 = t0 * t1, t0 = t0 ^ 2
			if (((currentWord >> j) & 0x1) == 0)
			{
				big_multiplication(&t1, t1, *z);
				big_squaring(z, *z);
			}
			// if ni = 1 -> t0 = t0 * t1, t1 = t1 ^2 
			else
			{
				big_multiplication(z, *z, t1);
				big_squaring(&t1, t1);
			}
			//mod
			big_mod(&t1, t1, y);
			big_mod(z, *z, y);
		}
	}

	big_delete(&t1);
	return SUCCESS;
}

/**
 * greatest common divisor : gcd(x, y) = z.
 *
 * \param z : address of bigint z (can be NULL)
 * \param x : bigint x (can't be NULL) (x >= 0)
 * \param y : bigint y (can't be NULL) (y >= 0)
 * \return : ErrorMessage
 */
ErrorMessage big_gcd(bigint** z, bigint* x, bigint* y)
{
	if (x == NULL || y == NULL)
		return FAIL_NULL;
	if (x->sign == NEGATIVE || y->sign == NEGATIVE)
		return FAIL_INVALID_INPUT;
	bigint* tmp = NULL;

	big_gcdRecursive(&tmp, x, y);

	big_assign(z, tmp);
	big_delete(&tmp);

	return SUCCESS;
}
// static function
static ErrorMessage big_gcdRecursive(bigint** z, bigint* x, bigint* y)
{
	if (big_is_zero(y))
		big_assign(z, x);
	else
	{
		bigint* tmp = NULL;
		// tmp = x % y
		big_mod(&tmp, x, y);
		// gcd(x, y) =  gcd(y, x % y)
		big_gcdRecursive(z, y, tmp);

		big_delete(&tmp);
	}

	return SUCCESS;
}

/**
 * extended euclidean algorhitm : compute a integeral solution(x,y) of ax + by = gcd(a,b) and gcd(a,b).
 *
 * \param d : address of bigint gcd(a, b) (can be NULL)
 * \param x : address of bigint x (can be NULL)
 * \param y : address of bigint y (can be NULL)
 * \param a : bigint a (can't be NULL) (a >= 0)
 * \param b : bigint b (can't be NULL) (b >= 0)
 * \return ErrorMessage
 */
ErrorMessage big_xgcd(bigint** d, bigint** x, bigint** y, bigint* a, bigint* b)
{
	if (a == NULL || b == NULL)
		return FAIL_NULL;
	if (a->sign == NEGATIVE || b->sign == NEGATIVE)
		return FAIL_INVALID_INPUT;

	bigint* tmpD = NULL;
	bigint* tmpX = NULL;
	bigint* tmpY = NULL;
	big_set_zero(&tmpD);
	big_set_zero(&tmpX);
	big_set_zero(&tmpY);

	big_xgcdRecursive(&tmpD, &tmpX, &tmpY, a, b);

	big_assign(d, tmpD);
	big_assign(x, tmpX);
	big_assign(y, tmpY);
	big_delete(&tmpD);
	big_delete(&tmpX);
	big_delete(&tmpY);

	return SUCCESS;
}
// static function
static ErrorMessage big_xgcdRecursive(bigint** d, bigint** x, bigint** y, bigint* a, bigint* b)
{
	if (big_is_zero(b))
	{
		big_assign(d, a);
		big_set_one(x);
		big_set_zero(y);
	}
	else
	{
		// assign
		bigint* x0 = NULL;
		bigint* y0 = NULL;
		bigint* tmpR = NULL;
		bigint* tmpQ = NULL;

		big_division(&tmpQ, &tmpR, a, b);

		// xgcd(d, u, v, a, b) = xgcd(d, u0, v0, b, a%b)
		big_xgcdRecursive(d, &x0, &y0, b, tmpR);

		// u = v0
		big_assign(x, y0);

		// v = u0 - v0 * (a / b)
		big_multiplication(&tmpQ, y0, tmpQ);
		big_subtraction(y, x0, tmpQ);

		// del
		big_delete(&tmpQ);
		big_delete(&tmpR);
		big_delete(&x0);
		big_delete(&y0);
	}
	return SUCCESS;
}