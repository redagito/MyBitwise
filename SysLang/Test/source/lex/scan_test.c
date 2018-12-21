#include "testy.h"

#include <syl/lex/scan.h>

#define TESTY_EPSILON 0.00000001

TESTY_TEST(scan)
{
	scan_init("test", "");
	TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);
	scan_next();
	TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);
	scan_next();
	TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);
	scan_next();
	TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);

	scan_init("test", "1");
	TESTY_CHECK(scan_get_token()->token.type == TOKEN_INT);
	TESTY_CHECK(scan_get_token()->token.int_val == 1);
	scan_next();
	TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);

	scan_init("test", "1234");
	TESTY_CHECK(scan_get_token()->token.type == TOKEN_INT);
	TESTY_CHECK(scan_get_token()->token.int_val == 1234);
	scan_next();
	TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);

	scan_init("test", "1234 1234");
	TESTY_CHECK(scan_get_token()->token.type == TOKEN_INT);
	TESTY_CHECK(scan_get_token()->token.int_val == 1234);
	scan_next();
	TESTY_CHECK(scan_get_token()->token.type == TOKEN_INT);
	TESTY_CHECK(scan_get_token()->token.int_val == 1234);
	scan_next();
	TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);

	scan_init("test", ".1");
	TESTY_CHECK(scan_get_token()->token.type == TOKEN_FLOAT);
	TESTY_CHECK(scan_get_token()->token.float_val + TESTY_EPSILON > 0.1f);
	TESTY_CHECK(scan_get_token()->token.float_val - TESTY_EPSILON < 0.1f);
	scan_next();
	TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);
}