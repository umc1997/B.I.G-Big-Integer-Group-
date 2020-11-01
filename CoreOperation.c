#include "CoreOperation.h"

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
ErrorMessage big_set_by_array(bigint** x, int sign, word* a, int wordlen) {
	big_new(x, sign, wordlen);
	for (int i = 0; i < wordlen; i++)
		(*x)->a[i] = a[i];
	big_refine(*x);
	return SUCCESS;
}
ErrorMessage big_set_by_string(bigint** x, int sign, char* str, int base) {
	if (base != 2 && base != 10 && base != 16)
		return FAIL_INVALID_BASE;
	int strLen = (int)strlen(str);
	for (int i = 0; i < strLen; i++)
	{
		char currentChar = *(str + i);
		if (!IsValidChar(currentChar, base))
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
ErrorMessage big_show_hex(bigint* x) {
	if (x == NULL)
		return FAIL_NULL;
	int i;
	int wordlen = x->wordlen;
	if (x->sign)
		printf("sign : negative,       ");
	else
		printf("sign : non - negative, ");
	
	for (i = wordlen - 1; i >= 0; i--)
	{
#if WORD_UNIT==8
		printf("%02x:", x->a[i]);
#elif WORD_UNIT==32
		printf("%08x", x->a[i]);
#elif WORD_UNIT==64
		printf("%016llx:", x->a[i]);
#endif
	}
	printf("\n");
	return SUCCESS;
}
ErrorMessage big_show_dec(bigint* x) //TODO , current max value = 2^64-1
{
	if (x == NULL)
		return FAIL_NULL;
	int i;
	int wordlen = x->wordlen;
	if (x->sign)
		printf("sign : negative,       ");
	else
		printf("sign : non - negative, ");
	unsigned long long output = 0;
	for (i = wordlen - 1; i >= 0; i--)
	{
#if WORD_UNIT==8
		output <<= 8;
#elif WORD_UNIT==32
		output <<= 32;
#elif WORD_UNIT==64
		output <<= 64;
#endif
		output += (x->a[i]);
	}
	printf("%llu\n", output);
	return SUCCESS;
}
ErrorMessage big_show_dec_for_each_word(bigint* x) {
	if (x == NULL)
		return FAIL_NULL;
	int i;
	int wordlen = x->wordlen;
	if (x->sign)
		printf("sign : negative,       ");
	else
		printf("sign : non - negative, ");
	for (i = wordlen - 1; i >= 0; i--)
	{
#if WORD_UNIT==8
		printf("%03d:", x->a[i]);
#elif WORD_UNIT==32
		printf("%010u:", x->a[i]);
#elif WORD_UNIT==64
		printf("%020llu:", x->a[i]);
#endif
	}
	printf("\n");
	return SUCCESS;
}
ErrorMessage big_show_bin(bigint* x) {
	if (x == NULL)
		return FAIL_NULL;
	int i;
	int wordlen = x->wordlen;
	if (x->sign)
		printf("sign : negative,       ");
	else
		printf("sign : non - negative, ");
	for (i = wordlen - 1; i >= 0; i--)
	{
		word current = x->a[i];
		int length = WORD_UNIT;
		while (length--)
		{
			printf("%d", (current >> length) & 0x1);
		}
		printf(":");
	}
	printf("\n");
	return SUCCESS;
}
ErrorMessage big_refine(bigint* x) {
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
		word *reallocWords = (word*)realloc(x->a, sizeof(word) * newWordlen);
		if (reallocWords == NULL)
			return FAIL_OUT_OF_MEMORY;
		else
			x->a = reallocWords;
	}
	if ((x->wordlen == 1) && (x->a == 0x0))
		x->sign = NON_NEGATIVE;
	return SUCCESS;
}
ErrorMessage big_assign(bigint** dst, bigint* src) {
	if (*dst == src)return SUCCESS;
	if (*dst != NULL)
		big_delete(dst);
	big_new(dst, src->sign, src->wordlen);
	memcpy((*dst)->a, src->a, sizeof(word) * (src->wordlen));
	return SUCCESS;
}
ErrorMessage big_gen_rand(bigint** x, int sign, int wordlen)
{
	big_new(x, sign, wordlen);
	array_rand((*x)->a, wordlen);
	big_refine(*x);
	return SUCCESS;
}
int big_get_wordlen(bigint* x) {
	if (x == NULL)
		return FAIL_NULL;
	return (x->wordlen);
}
int big_get_bitlen(bigint* x) {
	if (x == NULL)
		return FAIL_NULL;
#if WORD_UNIT == 8
	return (x->wordlen) << 3;
#elif WORD_UNIT == 32
	return (x->wordlen) << 5;
#elif WORD_UNIT == 64
	return (x->wordlen) << 6;
#endif
}
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
int big_get_sign(bigint* x) {
	if (x == NULL)
		return FAIL_NULL;
	return (x->sign);
}
ErrorMessage big_flip_sign(bigint** x) {
	if (x == NULL)
		return FAIL_NULL;
	if (big_is_zero(*x))
		return SUCCESS;
	(*x)->sign = !((*x)->sign);
	return SUCCESS;
}
ErrorMessage big_set_zero(bigint** x) {
	big_new(x, NON_NEGATIVE, 1);
	(*x)->a[0] = 0x0;
	return SUCCESS;
}
ErrorMessage big_set_one(bigint** x) {
	big_new(x, NON_NEGATIVE, 1);
	(*x)->a[0] = 0x1;
	return SUCCESS;
}
int big_is_zero(bigint* x) {
	if (x == NULL)
		return FAIL_NULL;
	big_refine(x);
	return (x->sign == NON_NEGATIVE) && (x->wordlen == 1) && (x->a[0] == 0x0);
}
int big_is_one(bigint* x) {
	if (x == NULL)
		return FAIL_NULL;
	big_refine(x);
	return (x->sign == NON_NEGATIVE) && (x->wordlen == 1) && (x->a[0] == 0x1);
}
int big_is_minus_one(bigint* x)
{
	if (x == NULL)
		return FAIL_NULL;
	big_refine(x);
	return (x->sign == NEGATIVE) && (x->wordlen == 1) && (x->a[0] == 0x1);
	
}
int big_compare(bigint* x, bigint* y) {
	if (x == NULL || y == NULL)
		return FAIL_NULL;
	big_refine(x);
	big_refine(y);
	int xSign = x->sign;
	int ySign = y->sign;
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
					return (big_compareAbs(x, y));
				}
			}
		}
		//both are negative
		else
		{
			return -big_compareAbs(x, y);
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
int big_compareAbs(bigint* x, bigint* y)
{
	int xWordlen = x->wordlen;
	int yWordlen = y->wordlen;
	if (xWordlen < yWordlen)
		return SMALLER;
	else if (xWordlen > yWordlen)
		return BIGGER;
	else
	{
		int i;
		word* xWords = x->a;
		word* yWords = y->a;
		for (i = xWordlen - 1; i >= 0; i--)
		{
			if (xWords[i] > yWords[i])
				return BIGGER;
			else if (xWords[i] < yWords[i])
				return SMALLER;
		}
		return EQUAL;
	}
}
ErrorMessage big_bit_left_shift(bigint** dst, bigint* src, int count) {
	if (src == NULL)
		return FAIL_NULL;
	if (count == 0)
	{
		big_assign(dst, src);
		return SUCCESS;
	}
	bigint* tmp = NULL;

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
	bigint* tmp = NULL;

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

ErrorMessage big_addition(bigint** z, bigint* x, bigint* y)
{
	if (x == NULL || y == NULL)
		return FAIL_NULL;
	// alloc
	bigint* tmp = NULL;
	big_new(&tmp, NON_NEGATIVE, 1);
	big_refine(x);
	big_refine(y);
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
			big_substraction(&tmp, y, absX);
			big_delete(&absX);
		}
		// x > 0 && y < 0 -> x + y = x - absy
		else if ((xSign == NON_NEGATIVE) && (ySign == NEGATIVE))
		{
			bigint* absY = NULL;
			big_assign(&absY, y);
			big_flip_sign(&absY);
			big_substraction(&tmp, x, absY);
			big_delete(&absY);
		}
		else
		{
			big_additionAbs(&tmp, x, y);
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
ErrorMessage big_additionAbs(bigint** z, bigint* x, bigint* y)
{
	word* xWords = x->a;
	word* yWords = y->a;
	int xWordlen = x->wordlen;
	int yWordlen = y->wordlen;

	int newWordlen = MAX(xWordlen, yWordlen);
	newWordlen += 1;

	(*z)->sign = NON_NEGATIVE;
	(*z)->wordlen = newWordlen;
	word* reallocWords = (word*)realloc((*z)->a, newWordlen * sizeof(word));
	if (reallocWords == NULL)
		return FAIL_OUT_OF_MEMORY;
	else
		(*z)->a = reallocWords;

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
		(*z)->a[i] = sum; //warning

		carryFromDown = carryToUp;
		carryToUp = 0;
	}
	big_refine(*z);

	return SUCCESS;
}

ErrorMessage big_substraction(bigint** z, bigint* x, bigint* y)
{
	if (x == NULL || y == NULL)
		return FAIL_NULL;
	//alloc
	bigint* tmp = NULL;
	big_new(&tmp, NON_NEGATIVE, 1);
	big_refine(x);
	big_refine(y);

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
			big_additionAbs(&tmp, x, y);
			big_flip_sign(&tmp);
		}
		// x > 0 && y < 0 -> x - y = x + absy
		else if ((xSign == NON_NEGATIVE) && (ySign == NEGATIVE))
		{
			big_additionAbs(&tmp, x, y);
		}
		else if ((xSign == NON_NEGATIVE) && (ySign == NON_NEGATIVE))
		{
			// x > y > 0 -> x - y = subAbs(x,y)
			if (comp == BIGGER)
				big_substractionAbs(&tmp, x, y);
			// y > x > 0 -> x - y = -subAbs(y,x)
			else
			{
				big_substractionAbs(&tmp, y, x);
				big_flip_sign(&tmp);
			}
		}
		else
		{
			// 0 > x > y -> x - y = (absy) - (absx) = subAbs(y,x)
			if (comp == BIGGER)
				big_substractionAbs(&tmp, y, x);
			// 0 > y > x -> x - y = -(-x) + (-y) = - (absx - absy) = -subAbs(x,y)
			else
			{
				big_substractionAbs(&tmp, x, y);
				big_flip_sign(&tmp);
			}
		}

	}
	big_assign(z, tmp);
	big_delete(&tmp);
	return SUCCESS;
}
ErrorMessage big_substractionAbs(bigint** z, bigint* x, bigint* y)
{
	word* xWords = x->a;
	word* yWords = y->a;
	int xWordlen = x->wordlen;
	int yWordlen = y->wordlen;
	int newWordlen = MAX(xWordlen, yWordlen);

	(*z)->sign = NON_NEGATIVE;
	(*z)->wordlen = newWordlen;
	word* reallocWords = (word*)realloc((*z)->a, newWordlen * sizeof(word));
	if (reallocWords == NULL)
		return FAIL_OUT_OF_MEMORY;
	else
		(*z)->a = reallocWords;


	int borrow = 0;
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
		(*z)->a[i] = dif; //warning
	}
	big_refine(*z);

	return SUCCESS;
}

ErrorMessage big_multiplication(bigint** z, bigint* x, bigint* y) 
{
	if (x == NULL || y == NULL)
		return FAIL_NULL;
	// alloc
	bigint* tmp = NULL;
	big_new(&tmp, NON_NEGATIVE, 1);
	
	big_refine(x);
	big_refine(y);
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
	else
	{	
		// tmp = |x| * |y|
		if (x->sign == NON_NEGATIVE)
		{
			if (y->sign == NON_NEGATIVE)
				big_multiplicationAbs(&tmp, x, y);
			else
			{
				bigint* absY = NULL;
				big_assign(&absY, y);
				absY->sign = NON_NEGATIVE;
				big_multiplicationAbs(&tmp, x, absY);
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
				big_multiplicationAbs(&tmp, absX, y);
			}
			else
			{
				bigint* absY = NULL;
				big_assign(&absY, y);
				absY->sign = NON_NEGATIVE;
				big_multiplicationAbs(&tmp, absX, absY);
				big_delete(&absY);
			}
			big_delete(&absX);
		}
		// if x * y < 0 -> negative
		if (big_get_sign(x) != big_get_sign(y))
			tmp->sign = NEGATIVE;
		// if x * y > 0 -> non_negative
		else
			tmp->sign = NON_NEGATIVE;
	}
	big_assign(z, tmp);
	big_delete(&tmp);
	return SUCCESS;

}
ErrorMessage big_multiplicationAbs(bigint** z, bigint* x, bigint* y)
{
	int newWordlen = x->wordlen + y->wordlen;

	(*z)->sign = NON_NEGATIVE;
	(*z)->wordlen = newWordlen;
	word* reallocWords = (word*)realloc((*z)->a, newWordlen * sizeof(word));
	if (reallocWords == NULL)
		return FAIL_OUT_OF_MEMORY;
	else
		(*z)->a = reallocWords;

	big_multiplicationKaratsuba(z, x, y);

	big_refine(*z);
	return SUCCESS;
}
ErrorMessage big_multiplicationKaratsuba(bigint** z, bigint* x, bigint* y)
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
		unsigned int l = MAX(xWordlen, yWordlen);
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
		bigint* S = NULL;
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
		big_word_left_shift(&R, T1, 2 * l);
		big_addition(&R, R, T0);
		
		// S1 = A0 - A1, S0 = B1 - B0
		big_substraction(&S1, A0, A1);
		big_substraction(&S0, B1, B0);
		
		// S = S1 * S0
		int isNegative = (S1->sign != S0->sign);
		S0->sign = NON_NEGATIVE;
		S1->sign = NON_NEGATIVE;
		big_multiplicationKaratsuba(&S, S0, S1);
		if (isNegative)
			S->sign = NEGATIVE;

		// AB = R + (S + T1 + T0)<<l
		big_addition(&S, S, T1);
		big_addition(&S, S, T0);
		big_word_left_shift(&S, S, l);
		big_addition(&S, S, R);
	
		// assign
		big_assign(z, S);

		//delete
		big_delete(&A1);
		big_delete(&A0);
		big_delete(&B1);
		big_delete(&B0);
		big_delete(&T1);
		big_delete(&T0);
		big_delete(&S1);
		big_delete(&S0);
		big_delete(&S);
		big_delete(&R);
	}
	return SUCCESS;
}
ErrorMessage big_multiplicationSchoolBook(bigint** z, bigint* x, bigint* y)
{
	if (x == NULL || y == NULL)
		return FAIL_NULL;

	word* xWords = x->a;
	word* yWords = y->a;
	int xWordlen = x->wordlen;
	int yWordlen = y->wordlen;

	// alloc
	bigint* tmp = NULL;
	big_set_zero(&tmp);
	// mul and add
	for (int i = 0; i < xWordlen; i++)
	{
		for (int j = 0; j < yWordlen; j++)
		{
			// T = (Aj * Bi) << w(i+j)
			bigint* T = NULL;
			big_new(&T, NON_NEGATIVE, i + j + 2);
			for (int k = 0; k < i + j + 2; k++)
				T->a[k] = 0;
			word A = 0, B = 0;
			wordMultiplication(&A, &B, xWords[i], yWords[j]);
			T->a[i + j + 1] = A;
			T->a[i + j] = B;

			// tmp = tmp + T
			big_additionAbs(&tmp, tmp, T);

			big_delete(&T);
		}
	}
	big_assign(z, tmp);
	big_delete(&tmp);
	return SUCCESS;
}
ErrorMessage big_multiplicationConst(bigint** z, bigint* x, const int y)
{
	if (x == NULL)
		return FAIL_NULL;
	int constant = y;
	//alloc
	bigint* tmp = NULL;
	big_assign(&tmp, x);
	if (constant == 0)
	{
		big_set_zero(&tmp);
	}
	else 
	{
		if (constant < 0)
		{
			big_flip_sign(&tmp);
			constant *= -1;
		}
		bigint* tmp2 = NULL;
		big_set_zero(&tmp2);
		while (constant != 1)
		{
			// if constant = c' * 2
			// x * constant = 2x * c' = bit_left_shift(x) * c'
			if (constant % 2 == 0)
			{
				big_bit_left_shift(&tmp, tmp, 1);
			}
			// if constant = c' * 2 + 1
			// x * constant = 2x * c' + x  = bit_left_shift(x) * c' + x
			else
			{
				big_addition(&tmp2, tmp, tmp2);
				big_bit_left_shift(&tmp, tmp, 1);
			}
			// constant <- c'
			constant /= 2;
		}
		big_addition(&tmp, tmp, tmp2);
		big_delete(&tmp2);
	}
	big_assign(z, tmp);
	big_delete(&tmp);
	return SUCCESS;
}