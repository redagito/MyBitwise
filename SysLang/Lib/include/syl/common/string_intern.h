#pragma once

#include <stdint.h>

/**
* String interning
*/

typedef const char* interned_string_t;

typedef struct intern_t
{
	size_t length;
	struct intern_t* next;
	char str[];
} intern_t;

interned_string_t string_intern_range(const char* start, const char* end);

interned_string_t string_intern(const char* str);