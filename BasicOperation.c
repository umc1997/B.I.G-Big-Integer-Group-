#include "BasicOperation.h"

ErrorMessage array_rand(word* dst, int wordlen) {
	byte* p = (byte*)dst;
	int cnt = wordlen * sizeof(word);
	while (cnt > 0)
	{
		*p = rand() & 0xff;
		p++;
		cnt--;
	}
	return SUCCESS;
}
ErrorMessage array_fill(word* arr, int arrlen, int val)
{
	int i;
	for (i = 0; i < arrlen; i++)
		*(arr + i) = val;
	return SUCCESS;
}
unsigned int hex2int(char c)
{
	if (c >= 'a' && c <= 'z')
	{
		return c - 'a' + 10;
	}
	else if (c >= 'A' && c <= 'Z')
	{
		return c - 'A' + 10;
	}
	else if (c >= '0' && c <= '9')
	{
		return c - '0';
	}
	else
	{
		return 0;
	}
}
int IsValidChar(char c, int base)
{
	if (base == 2)
	{
		if (c != '0' && c != '1')
			return 0;
	}
	else if (base == 10)
	{
		if (!(c >= '0' && c <= '9'))
			return 0;
	}
	else if (base == 16)
	{
		if (c >= 'a' && c <= 'z') {}
		else if (c >= 'A' && c <= 'Z') {}
		else if (c >= '0' && c <= '9') {}
		else
			return 0;
	}
	else
		return 0;
	return 1;
}

ErrorMessage wordMultiplication(word* C1, word* C0, word A, word B)
{
	// A = A0 + A1 * W^(1/2), B = B0 + B1 * W^(1/2)
	word A1, A0, B1, B0, T1, T0, T;
	unsigned int shiftUnit = WORD_UNIT >> 1;
	A1 = A >> shiftUnit;
	A0 = A - (A1 << shiftUnit);
	B1 = B >> shiftUnit;
	B0 = B - (B1 << shiftUnit);

	// A * B = A1B1 * W + (A0B1 + A1B0) * W^(1/2) + A0B0
	// W^(1/2) part : A0B1 + A1B0
	T1 = A1 * B0;
	T0 = A0 * B1;

	T0 += T1;
	T1 = (T0 < T1); // carry

	// W and 1 part : A1B1 and A0B0 
	*C1 = A1 * B1;
	*C0 = A0 * B0;

	// final
	T = *C0;
	*C0 += (T0 << shiftUnit);
	*C1 += (T1 << shiftUnit) + (T0 >> shiftUnit) + ((*C0) < T);

	return SUCCESS;
}