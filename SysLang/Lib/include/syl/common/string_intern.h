#pragma once

#include <stdint.h>

/**
* String interning
* Canonicalize different representations of the same string content.
*/

/**
* Returns interned copy of string.
*/
const char* string_intern(const char* str);

/**
* Returns interned copy of substring.
*/
const char* string_intern_range(const char* start, const char* end);