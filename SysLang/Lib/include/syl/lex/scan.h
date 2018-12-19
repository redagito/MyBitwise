#pragma once

#include <stdbool.h>

#include "syl/lex/token.h"

/**
* Source code scanning
*/

/**
* Source code position
*/
typedef struct source_position_t
{
	const char* name;
	int line;
} source_position_t;

/**
* Token with source code and position info
*/
typedef struct scan_token_t
{
	token_t token;

	source_position_t position;
	const char* start;
	const char* end;
} scan_token_t;

/**
* Initializes scanner with new stream source
* name - file name or identifier of the stream
* stream - pointer to beginning of the source code buffer
*/
void scan_init(const char* name, const char* stream);

/**
* Scans next token from stream
*/
void scan_next();