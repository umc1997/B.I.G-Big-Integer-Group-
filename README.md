# The Notorious B.I.G-Big-Integer-Group-

#You can change unit of word in config.h file.
Usage:
1. Create Big Integer
	This library provides big integer declaration using strings or arrays. (String provides only hexadecimal, decimal, binary) 
	example source codes:
	1-1.if you want to create a big integer using hexadecimal string.(most recommended)
		bigint* a = NULL;
		big_set_by_string(&a, NON_NEGATIVE, "123456789abcdef", 16); //Don't write prefix in front of string
		
	1-2.if you want to create a big integer using binary string.
		bigint* a = NULL;
		big_set_by_string(&a, NON_NEGATIVE, "1010110010100011", 2); //Don't write prefix in front of string
		
	1-3.if you want to create a big integer using decimal string.
		bigint* a = NULL;
		big_set_by_string(&a, NON_NEGATIVE, "1234567890", 10); //Don't write prefix in front of string
		
	1-4.if you want to create a big integer 0x123456789abcdef using array.
		bigint* a = NULL;
		word aWords[8] = {0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01}; //8-bit
		big_set_by_array(&a, NON_NEGATIVE, aWords, 8);
		
	1-5.if you want to generate a 10-word length random big integer.
		bigint* a = NULL;
		big_gen_rand(&a, NON_NEGATIVE,10);
	
	1-6.if you want to assign a big integer to another big integer.
		bigint* a = NULL;
		bigint* b = NULL;
		/*some operation*/
		/*you want to assign a to b*/
		big_assign(&b, a);
		
2. Delete Big Integer
	example source codes:
		bigint* a = NULL;
		/*some operation*/
		big_delete(&a); //Don't forget free your memory!!
		
3. Show Big Integer
	This library provides 3 big integer print methods. hexadecimal, decimal, binary.
	example source codes:
	1-1.if you want to print the big integer in hexadecimal.(most recommended)
		bigint* a = NULL;
		/*some operation*/
		big_show_hex(a);
		
	1-2.if you want to print the big integer in binary.
		bigint* a = NULL;
		/*some operation*/
		big_show_bin(a);
		
	1-3.if you want to print the big integer in decimal. (don't recommended)
		bigint* a = NULL;
		/*some operation*/
		big_show_dec(a);
	
4. Arithmetic Operation
	This library provides several arithmetic operations.
	addition, substraction, multiplication, squaring, division, modular exponentiation, euclidean algorithm, extended euclidean algorithm, modular inverse
	1-1.addition
		bigint* a = NULL;
		bigint* b = NULL;
		bigint* c = NULL;
		big_set_by_string(&a, NON_NEGATIVE, "123456789", 16);
		big_set_by_string(&b, NON_NEGATIVE, "987654321", 16);
		big_addition(&c, a, b); // c = a + b
		
	1-2.substraction
		bigint* a = NULL;
		bigint* b = NULL;
		bigint* c = NULL;
		big_set_by_string(&a, NON_NEGATIVE, "123456789", 16);
		big_set_by_string(&b, NON_NEGATIVE, "987654321", 16);
		big_substraction(&c, a, b); // c = a - b
		
	1-3.multiplication
		bigint* a = NULL;
		bigint* b = NULL;
		bigint* c = NULL;
		big_set_by_string(&a, NON_NEGATIVE, "123456789", 16);
		big_set_by_string(&b, NON_NEGATIVE, "987654321", 16);
		big_multiplication(&c, a, b); // c = a * b
		
	1-4.squaring
		bigint* a = NULL;
		bigint* c = NULL;
		big_set_by_string(&a, NON_NEGATIVE, "123456789", 16);
		big_squaring(&c, a); // c = a * a
		
	1-5.division
		bigint* a = NULL;
		bigint* b = NULL;
		bigint* q = NULL;
		bigint* r = NULL;
		big_set_by_string(&a, NON_NEGATIVE, "123456789", 16);
		big_set_by_string(&b, NON_NEGATIVE, "987654321", 16);
		big_division(&q, &r, a, b); // a / b = q ... r  ->  a = b * q + r (0<= r < b)
		
	1-6.modular exponentiation
		bigint* a = NULL;
		bigint* n = NULL;
		bigint* b = NULL;
		bigint* c = NULL;
		big_set_by_string(&a, NON_NEGATIVE, "123456789", 16);
		big_set_by_string(&n, NON_NEGATIVE, "10001", 16);
		big_set_by_string(&b, NON_NEGATIVE, "91", 16);
		big_mod_exp(&c, a, n, b); //c = a ^ n mod b
		
	1-7.euclidean algorithm
		bigint* a = NULL;
		bigint* b = NULL;
		bigint* d = NULL;
		big_set_by_string(&a, NON_NEGATIVE, "6", 16);
		big_set_by_string(&b, NON_NEGATIVE, "3", 16);
		big_gcd(&d, a, b); // d = gcd(a,b)
		
	1-8.extended euclidean algorithm
		bigint* a = NULL;
		bigint* b = NULL;
		bigint* u = NULL;
		bigint* v = NULL;
		bigint* d = NULL;
		big_set_by_string(&a, NON_NEGATIVE, "6", 16);
		big_set_by_string(&b, NON_NEGATIVE, "3", 16);
		big_xgcd(&d, &u, &v, a, b); // d = gcd(a, b), find u, v such that a * u + b * v = d
		
	1-9.modular inverse
		bigint* a = NULL;
		bigint* b = NULL;
		bigint* c = NULL;
		big_set_by_string(&a, NON_NEGATIVE, "3", 16);
		big_set_by_string(&b, NON_NEGATIVE, "5", 16);
		big_mod_inverse(&c, a, b); // find c such that a * c mod b = 1
	
5. RSA
	This library provides RSA key generate function, RSA encipher function and RSA decipher function.
	You can change RSA bit in config.h file.
	1-1.key generate
		bigint* publicKey = NULL;
		bigint* privateKey = NULL;
		big_RSA_key_gen(&publicKey, &privateKey);
		
	1-2.encipher message
		bigint* message = NULL;
		bigint* cipherText = NULL;
		/* input message */
		big_RSA_encipher(&cipherText, message, publicKey);
		
	1-3.decipher message
		bigint* cipherText = NULL;
		bigint* decCipherText = NULL;
		/* input cipherText */
		big_RSA_decipher(&decCipherText, cipherText, publicKey, privateKey);
