#pragma once

#include <stdint.h>

/**
* String interning
* Canonicalize different representations of the same string content.
*/

/**
* Returns interned copy of string.
*/
const char* intern_string(const char* str);

/**
* Returns interned copy of substring.
*/
const char* intern_string_range(const char* start, const char* end);