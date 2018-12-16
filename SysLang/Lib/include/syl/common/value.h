#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
* Any type value
*/

typedef union value_t
{
	bool b;
	char c;
	unsigned char uc;
	signed char sc;

	short s;
	unsigned short us;

	int i;
	unsigned int ui;

	long l;
	unsigned long ul;

	long long ll;
	unsigned long long ull;

	uintptr_t p;
} value_t;