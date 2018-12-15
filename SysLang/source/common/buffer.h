#pragma once

#include <stdint.h>

/**
* Stretchy buffer
*/

typedef struct buffer_header_t
{
	size_t length;
	size_t capacity;
	char buffer[];
} buffer_header_t;