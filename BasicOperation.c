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
void reverseStr(char* str)
{
	if (str == 0)
	{
		return;
	}

	/* skip empty string */
	if (*str == 0)
	{
		return;
	}

	/* get range */
	char* start = str;
	char* end = start + strlen(str) - 1; /* -1 for \0 */
	char temp;

	/* reverse */
	while (end > start)
	{
		/* swap */
		temp = *start;
		*start = *end;
		*end = temp;

		/* move */
		++start;
		--end;
	}
}
char digit2char(word a)
{
	if (a == 0) return '0';
	else if (a == 1) return '1';
	else if (a == 2) return '2';
	else if (a == 3) return '3';
	else if (a == 4) return '4';
	else if (a == 5) return '5';
	else if (a == 6) return '6';
	else if (a == 7) return '7';
	else if (a == 8) return '8';
	else if (a == 9) return '9';
	else return '\0';
}
unsigned int hex2int(char c)
{
	if (c >= 'a' && c <= 'z')
	{
		return (int)c - 'a' + 10;
	}
	else if (c >= 'A' && c <= 'Z')
	{
		return (int)c - 'A' + 10;
	}
	else if (c >= '0' && c <= '9')
	{
		return (int)c - '0';
	}
	else
	{
		return 0;
	}
}
bool isValidChar(char c, int base)
{
	if (base == 2)
	{
		if (c != '0' && c != '1')
			return false;
	}
	else if (base == 10)
	{
		if (!(c >= '0' && c <= '9'))
			return false;
	}
	else if (base == 16)
	{
		if (c >= 'a' && c <= 'z') {}
		else if (c >= 'A' && c <= 'Z') {}
		else if (c >= '0' && c <= '9') {}
		else
			return false;
	}
	else
		return false;
	return true;
}

ErrorMessage wordMultiplication(word* C1, word* C0, word A, word B)
{
	// A = A0 + A1 * W^(1/2), B = B0 + B1 * W^(1/2)
	word A1 = 0, A0 = 0, B1 = 0, B0 = 0, T1 = 0, T0 = 0, T = 0;
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

ErrorMessage wordSquaring(word* C1, word* C0, word A)
{
	word A1, A0, T0, T1, T;
	unsigned int shiftUnit = WORD_UNIT >> 1;
	A1 = A >> shiftUnit;
	A0 = A - (A1 << shiftUnit);

	// A * A = A1^2 * W + 2A0A1 * W^(1/2) + A0^2
	// W^(1/2) part : 2A0A1
	T = A0 * A1;

	T1 = T >> (shiftUnit - 1);
	T0 = T << (shiftUnit + 1);

	// W and 1 part : A1^2 and A0^2
	*C0 = A0 * A0;
	*C1 = A1 * A1;

	//final
	*C0 = *C0 + T0;
	*C1 = *C1 + T1 + ((*C0) < T0);

	return SUCCESS;
}
ErrorMessage wordLongDivision(word* Q, word A1, word A0, word B)
{
	*Q = 0;
	word R = A1;
	int w = WORD_UNIT;

	for (int i = w - 1; i >= 0; i--)
	{
		if (R >= ((word)1 << (w - 1)))
		{
			(*Q) += ((word)1 << i);
			R <<= 1;
			R += ((A0 >> i) & 1);
			R -= B;
		}
		else
		{
			R <<= 1;
			R += ((A0 >> i) & 1);
			if (R >= B)
			{
				(*Q) += ((word)1 << i);
				R -= B;
			}
		}
	}

	return SUCCESS;
}