#include "syl/lex/scan.h"

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

#include "syl/common/log.h"

static uint8_t char_to_digit[256] = 
{
	['0'] = 0,
	['1'] = 1,
	['2'] = 2,
	['3'] = 3,
	['4'] = 4,
	['5'] = 5,
	['6'] = 6,
	['7'] = 7,
	['8'] = 8,
	['9'] = 9,
	['a'] = 10,['A'] = 10,
	['b'] = 11,['B'] = 11,
	['c'] = 12,['C'] = 12,
	['d'] = 13,['D'] = 13,
	['e'] = 14,['E'] = 14,
	['f'] = 15,['F'] = 15
};

static char escape_to_char[256] = 
{
	['0'] = '\0',
	['\''] = '\'',
	['"'] = '"',
	['\\'] = '\\',
	['n'] = '\n',
	['r'] = '\r',
	['t'] = '\t',
	['v'] = '\v',
	['b'] = '\b',
	['a'] = '\a',
};

typedef struct scan_state_t
{
	scan_token_t token;
	const char* stream;
	const char* line_start;
} scan_state_t;

// Active state of scanner
static scan_state_t scan_state;

// Builtin source code position
static source_position_t builtin_position = { .name = "<builtin>" };

static const char* scan_token_info()
{
	return token_info(&scan_state.token.token);
}

static void scan_log_internal(source_position_t position, const char* level, const char* fmt, va_list args)
{
	if (position.name == NULL)
	{
		position = builtin_position;
	}

	// TODO Should use log functions instead
	printf("%s(%i): %s: ", position.name, position.line, level);
	vprintf(fmt, args);
	printf("\n");
}

static void scan_log_warning(source_position_t position, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	scan_log_internal(position, "warning", fmt, args);
	va_end(args);
}

static void scan_log_error(source_position_t position, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	scan_log_internal(position, "error", fmt, args);
	va_end(args);
}

static void scan_log_fatal(source_position_t position, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	scan_log_internal(position, "fatal", fmt, args);
	va_end(args);
	exit(1);
}

// Log helpers for current scan token
#define scan_log_warning_here(...) (scan_log_warning(scan_state.token.position, __VA_ARGS__))
#define scan_log_error_here(...) (scan_log_error(scan_state.token.position, __VA_ARGS__))
#define scan_log_fatal_here(...) (scan_log_fatal(scan_state.token.position, __VA_ARGS__))

/**
* Provides information for current token
*/
static bool scan_is_type(token_type_t type)
{
	return scan_state.token.token.type == type;
}

static bool scan_is_eof()
{
	return scan_is_type(TOKEN_EOF);
}

// Use interned strings only
// TODO Rename 'name' to 'identifier'
static bool scan_is_name(const char* name)
{
	return scan_is_type(TOKEN_NAME) && scan_state.token.token.name == name;
}

static bool scan_is_keyword(const char* keyword)
{
	return scan_is_type(TOKEN_KEYWORD) && scan_state.token.token.name == keyword;
}

/**
* Attempt to match current token and retrieve next token on success
* Returns true if matched and false otherwise
*/
static bool scan_match_keyword(const char* keyword)
{
	if (!scan_is_keyword(keyword))
	{
		return false;
	}
	scan_next_token();
	return true;
}

static bool scan_match_type(token_type_t type)
{
	if (!scan_is_type(type))
	{
		return false;
	}
	scan_next_token();
	return true;
}

/**
* Same as scan_match_type but logs a fatal error on mismatch.
*/
static void scan_expect_type(token_type_t type)
{
	if (!scan_match_type(type))
	{
		scan_log_fatal_here("Expected token %s, got %s", token_type_name(type), scan_token_info());
	}
}

static unsigned long long scan_uintval(int base)
{
	unsigned long long value = 0;
	
	while (true)
	{
		char curr = *scan_state.stream;

		// Skip underscores in between digits
		if (curr == '_')
		{
			++scan_state.stream;
			continue;
		}

		// Map to decimal value
		int digit = char_to_digit[(unsigned char)curr];
		// '0' is mapped to 0 as is every char wich is not a digit
		if (digit == 0 && curr != '0')
		{
			// No valid digit, end of value
			break;
		}

		if (digit >= base)
		{
			scan_log_error_here("Digit '%c' is out of range for base %i", curr, base);
			// TODO Just set to 0 and proceed?
			digit = 0;
		}

		if (value > (ULLONG_MAX - digit) / base)
		{
			// Value would exceed maximum allowed value
			scan_log_error_here("Integer literal overflow");
			// Consume remaining digits
			// TODO Ignores digits from bases > 10
			while (isdigit(*scan_state.stream) || *scan_state.stream == '_')
			{
				++scan_state.stream;
			}
			value = 0;
			break;
		}

		value = value * base + digit;
		++scan_state.stream;
	}
	return value;
}

/**
* Try match lowercase char
* Moves stream forward on success
*/
static bool scan_match_lowercase(char c)
{
	if (tolower(*scan_state.stream) != c)
	{
		return false;
	}
	++scan_state.stream;
	return true;
}

static void scan_expect(char c)
{
	if (*scan_state.stream != c)
	{
		scan_log_fatal_here("Received '%c' while expecting '%c'", *scan_state.stream, c);
	}
	++scan_state.stream;
}

static void scan_int()
{
	int base = 10;
	const char* start_digits = scan_state.stream;

	// Might be hex, octal or binary value
	if (*scan_state.stream == '0')
	{
		++scan_state.stream;
		if (tolower(*scan_state.stream) == 'x')
		{
			// Hex value
			++scan_state.stream;
			scan_state.token.token.modifier = MOD_HEX;
			base = 16;
			start_digits = scan_state.stream;
		}
		else if (tolower(*scan_state.stream) == 'b')
		{
			// Binary value
			++scan_state.stream;
			scan_state.token.token.modifier = MOD_BIN;
			base = 2;
			start_digits = scan_state.stream;
		}
		else if (isdigit(*scan_state.stream))
		{
			// Octal value
			scan_state.token.token.modifier = MOD_OCT;
			base = 8;
			start_digits = scan_state.stream;
		}
	}

	// Scan remaining digits
	unsigned long long value = scan_uintval(base);

	// Check for digits scanned
	if (scan_state.stream == start_digits)
	{
		scan_log_error_here("Expected base %i digit, got '%c'", base, *scan_state.stream);
	}

	scan_state.token.token.type = TOKEN_INT;
	scan_state.token.token.int_val = value;

	// Scan suffix
	if (scan_match_lowercase('u'))
	{
		scan_state.token.token.suffix = SUFFIX_U;
		if (scan_match_lowercase('l'))
		{
			scan_state.token.token.suffix = SUFFIX_UL;
			if (scan_match_lowercase('l'))
			{
				scan_state.token.token.suffix = SUFFIX_ULL;
			}
		}
	}
	else if (scan_match_lowercase('l'))
	{
		scan_state.token.token.suffix = SUFFIX_L;
		if (scan_match_lowercase('l'))
		{
			scan_state.token.token.suffix = SUFFIX_LL;
		}
	}
}

static void scan_float()
{

}

static int scan_hex_escape()
{
	scan_expect('x');
	char curr = *scan_state.stream;
	
	// First digit (required)
	int value = char_to_digit[(unsigned char)curr];
	if (value == 0 && curr != '0')
	{
		scan_log_error_here("\\x requires at least 1 hex digit");
	}
	++scan_state.stream;

	// Second digit (optional)
	curr = *scan_state.stream;
	int digit = char_to_digit[(unsigned char)curr];
	if (value != 0 || curr == '0')
	{
		value = value * 16 + digit;
		if (value > 0xFF)
		{
			scan_log_error_here("\\x argument out of range");
			value = 0xFF;
		}
		++scan_state.stream;
	}

	return value;
}

static void scan_char()
{

}

static void scan_str()
{

}

void scan_init(const char* name, const char* stream)
{
	scan_state.stream = stream;
	scan_state.line_start = stream;

	// Default position name
	scan_state.token.position.name = name != NULL ? name : "<string>";
	scan_state.token.position.line = 1;

	// Scan first token from stream
	scan_next_token();
}

void scan_next()
{

}