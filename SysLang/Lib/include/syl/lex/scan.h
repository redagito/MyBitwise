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
* Initializes scanner with new stream source and scans first token
* name - file name or identifier of the stream
* stream - pointer to beginning of the source code buffer
*/
void scan_init(const char* name, const char* stream);

/**
* Scans next token from stream
*/
void scan_next();

/**
* Current token in active stream
*/
const scan_token_t* scan_get_token();

/**
* Provides information for current token
*/
bool scan_is_type(token_type_t type);

bool scan_is_eof();

// Use interned strings only
// TODO Rename 'name' to 'identifier'
bool scan_is_name(const char* name);

bool scan_is_keyword(const char* keyword);

/**
* Attempt to match current token and retrieve next token on success
* Returns true if matched and false otherwise
*/
bool scan_match_keyword(const char* keyword);

bool scan_match_type(token_type_t type);

/**
* Same as scan_match_type but logs a fatal error on mismatch.
*/
void scan_expect_type(token_type_t type);