#include "testy.h"

#include <syl/lex/scan.h>

#define TESTY_EPSILON 0.000001

TESTY_TEST(scan) {
  scan_init("test", "'c'");
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_INT);
  TESTY_CHECK(scan_get_token()->token.modifier == MOD_CHAR);
  scan_next();
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);

  scan_init("test", "'\\n'");
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_INT);
  TESTY_CHECK(scan_get_token()->token.modifier == MOD_CHAR);
  scan_next();
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);

  scan_init("test", "\"abcdefg\"");
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_STR);
  TESTY_CHECK(scan_get_token()->token.modifier == MOD_NONE);
  TESTY_CHECK(strcmp(scan_get_token()->token.str_val, "abcdefg") == 0);
  scan_next();
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);

  scan_init("test", ".");
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_DOT);
  scan_next();
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);

  scan_init("test", "...");
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_ELLIPSIS);
  scan_next();
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);

  scan_init("test", ".1");
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_FLOAT);
  TESTY_CHECK(scan_get_token()->token.float_val + TESTY_EPSILON > 0.1f);
  TESTY_CHECK(scan_get_token()->token.float_val - TESTY_EPSILON < 0.1f);
  scan_next();
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);

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

  scan_init("test", "12.34");
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_FLOAT);
  TESTY_CHECK(scan_get_token()->token.float_val + TESTY_EPSILON > 12.34f);
  TESTY_CHECK(scan_get_token()->token.float_val - TESTY_EPSILON < 12.34f);
  scan_next();
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);

  scan_init("test", "1e2");
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_FLOAT);
  TESTY_CHECK(scan_get_token()->token.float_val + TESTY_EPSILON > 100.f);
  TESTY_CHECK(scan_get_token()->token.float_val - TESTY_EPSILON < 100.f);
  scan_next();
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);

  scan_init("test", "1e-2");
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_FLOAT);
  TESTY_CHECK(scan_get_token()->token.float_val + TESTY_EPSILON > 0.01f);
  TESTY_CHECK(scan_get_token()->token.float_val - TESTY_EPSILON < 0.01f);
  scan_next();
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);

  scan_init("test", "abc123");
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_NAME);
  TESTY_CHECK(strcmp(scan_get_token()->token.name, "abc123") == 0);
  scan_next();
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);

  scan_init("test", "if");
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_KEYWORD);
  TESTY_CHECK(strcmp(scan_get_token()->token.name, "if") == 0);
  scan_next();
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);

  scan_init("test", "// testcomment\n123");
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_COMMENT);
  TESTY_CHECK(strcmp(scan_get_token()->token.comment_text, " testcomment") ==
              0);
  scan_next();
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_INT);
  TESTY_CHECK(scan_get_token()->token.int_val == 123);
  scan_next();
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_EOF);

  scan_init("test", "/* testcomment\n123 */ 456");
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_COMMENT);
  TESTY_CHECK(scan_get_token()->token.modifier == MOD_MULTILINE);
  TESTY_CHECK(
      strcmp(scan_get_token()->token.comment_text, " testcomment\n123 ") == 0);
  scan_next();
  TESTY_CHECK(scan_get_token()->token.type == TOKEN_INT);
  TESTY_CHECK(scan_get_token()->token.int_val == 456);
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
}