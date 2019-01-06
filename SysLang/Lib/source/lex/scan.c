#include "syl/lex/scan.h"

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "syl/common/buffer.h"
#include "syl/common/intern.h"
#include "syl/common/log.h"
#include "syl/lex/keywords.h"
#include "syl/lex/token.h"

static uint8_t char_to_digit[256] = {
    ['0'] = 0,  ['1'] = 1,  ['2'] = 2,  ['3'] = 3,  ['4'] = 4,  ['5'] = 5,
    ['6'] = 6,  ['7'] = 7,  ['8'] = 8,  ['9'] = 9,  ['a'] = 10, ['A'] = 10,
    ['b'] = 11, ['B'] = 11, ['c'] = 12, ['C'] = 12, ['d'] = 13, ['D'] = 13,
    ['e'] = 14, ['E'] = 14, ['f'] = 15, ['F'] = 15};

static char escape_to_char[256] = {
    ['0'] = '\0', ['\''] = '\'', ['"'] = '"',  ['\\'] = '\\', ['n'] = '\n',
    ['r'] = '\r', ['t'] = '\t',  ['v'] = '\v', ['b'] = '\b',  ['a'] = '\a',
};

typedef struct scan_state_t {
  scan_token_t token;
  const char *stream;
  const char *line_start;
} scan_state_t;

// Active state of scanner
static scan_state_t scan_state;

// Builtin source code position
static source_position_t builtin_position = {.name = "<builtin>"};

static const char *scan_token_info() {
  return token_info(&scan_state.token.token);
}

static void scan_log_internal(source_position_t position, const char *level,
                              const char *fmt, va_list args) {
  if (position.name == NULL) {
    position = builtin_position;
  }

  // TODO Should use log functions instead
  printf("%s(%i): %s: ", position.name, position.line, level);
  vprintf(fmt, args);
  printf("\n");
}

static void scan_log_warning(source_position_t position, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  scan_log_internal(position, "warning", fmt, args);
  va_end(args);
}

static void scan_log_error(source_position_t position, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  scan_log_internal(position, "error", fmt, args);
  va_end(args);
}

static void scan_log_fatal(source_position_t position, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  scan_log_internal(position, "fatal", fmt, args);
  va_end(args);
  exit(1);
}

// Log helpers for current scan token
#define scan_log_warning_here(...)                                             \
  (scan_log_warning(scan_state.token.position, __VA_ARGS__))
#define scan_log_error_here(...)                                               \
  (scan_log_error(scan_state.token.position, __VA_ARGS__))
#define scan_log_fatal_here(...)                                               \
  (scan_log_fatal(scan_state.token.position, __VA_ARGS__))

static unsigned long long scan_uintval(int base) {
  unsigned long long value = 0;

  while (true) {
    char curr = *scan_state.stream;

    // Skip underscores in between digits
    if (curr == '_') {
      ++scan_state.stream;
      continue;
    }

    // Map to decimal value
    int digit = char_to_digit[(unsigned char)curr];
    // '0' is mapped to 0 as is every char wich is not a digit
    if (digit == 0 && curr != '0') {
      // No valid digit, end of value
      break;
    }

    if (digit >= base) {
      scan_log_error_here("Digit '%c' is out of range for base %i", curr, base);
      // TODO Just set to 0 and proceed?
      digit = 0;
    }

    if (value > (ULLONG_MAX - digit) / base) {
      // Value would exceed maximum allowed value
      scan_log_error_here("Integer literal overflow");
      // Consume remaining digits
      // TODO Ignores digits from bases > 10
      while (isdigit(*scan_state.stream) || *scan_state.stream == '_') {
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

static bool scan_match_char(char c) {
  if (*scan_state.stream != c) {
    return false;
  }
  ++scan_state.stream;
  return true;
}

/**
 * Try match lowercase char
 * Moves stream forward on success
 */
static bool scan_match_lowercase(char c) {
  if (tolower(*scan_state.stream) != c) {
    return false;
  }
  ++scan_state.stream;
  return true;
}

static void scan_expect_char(char c) {
  if (*scan_state.stream != c) {
    scan_log_fatal_here("Received '%c' while expecting '%c'",
                        *scan_state.stream, c);
  }
  ++scan_state.stream;
}

static void scan_int() {
  int base = 10;
  const char *start_digits = scan_state.stream;

  // Might be hex, octal or binary value
  if (*scan_state.stream == '0') {
    ++scan_state.stream;
    if (tolower(*scan_state.stream) == 'x') {
      // Hex value
      ++scan_state.stream;
      scan_state.token.token.modifier = MOD_HEX;
      base = 16;
      start_digits = scan_state.stream;
    } else if (tolower(*scan_state.stream) == 'b') {
      // Binary value
      ++scan_state.stream;
      scan_state.token.token.modifier = MOD_BIN;
      base = 2;
      start_digits = scan_state.stream;
    } else if (isdigit(*scan_state.stream)) {
      // Octal value
      scan_state.token.token.modifier = MOD_OCT;
      base = 8;
      start_digits = scan_state.stream;
    }
  }

  // Scan remaining digits
  unsigned long long value = scan_uintval(base);

  // Check for digits scanned
  if (scan_state.stream == start_digits) {
    scan_log_error_here("Expected base %i digit, got '%c'", base,
                        *scan_state.stream);
  }

  scan_state.token.token.type = TOKEN_INT;
  scan_state.token.token.int_val = value;

  // Scan suffix
  if (scan_match_lowercase('u')) {
    scan_state.token.token.suffix = SUFFIX_U;
    if (scan_match_lowercase('l')) {
      scan_state.token.token.suffix = SUFFIX_UL;
      if (scan_match_lowercase('l')) {
        scan_state.token.token.suffix = SUFFIX_ULL;
      }
    }
  } else if (scan_match_lowercase('l')) {
    scan_state.token.token.suffix = SUFFIX_L;
    if (scan_match_lowercase('l')) {
      scan_state.token.token.suffix = SUFFIX_LL;
    }
  }
}

/**
 * Skip until end of digits in stream
 */
static void scan_skip_digits() {
  while (isdigit(*scan_state.stream)) {
    ++scan_state.stream;
  }
}

static void scan_float() {
  const char *start = scan_state.stream;

  // Validate structure of the floating point literal
  // Scan required part
  scan_skip_digits();
  scan_match_char('.');
  scan_skip_digits();

  // Scan optional exponent
  if (scan_match_lowercase('e')) {
    if (*scan_state.stream == '+' || *scan_state.stream == '-') {
      ++scan_state.stream;
    }
    if (!isdigit(*scan_state.stream)) {
      scan_log_error_here(
          "Expected digit after float literal exponent, found '%c'",
          *scan_state.stream);
    }
    scan_skip_digits();
  }

  // Float structure OK, parse
  // TODO Compare start with current stream pos to determine if
  // anything got actually scanned?
  double value = strtod(start, NULL);
  if (value == HUGE_VAL) {
    scan_log_error_here("Float literal overflow");
  }

  // Set token
  scan_state.token.token.type = TOKEN_FLOAT;
  scan_state.token.token.float_val = value;
  if (scan_match_lowercase('d')) {
    scan_state.token.token.suffix = SUFFIX_D;
  }
}

/**
 * Scans hexadecimal escape code
 * Example: x1F
 */
static int scan_hex_escape() {
  scan_expect_char('x');
  char curr = *scan_state.stream;

  // First digit (required)
  int value = char_to_digit[(unsigned char)curr];
  if (value == 0 && curr != '0') {
    scan_log_error_here("\\x requires at least 1 hex digit");
  }
  ++scan_state.stream;

  // Second digit (optional)
  curr = *scan_state.stream;
  int digit = char_to_digit[(unsigned char)curr];
  if (value != 0 || curr == '0') {
    value = value * 16 + digit;
    if (value > 0xFF) {
      scan_log_error_here("\\x argument out of range");
      value = 0xFF;
    }
    ++scan_state.stream;
  }

  return value;
}

/**
 * Character literal
 * Examples: 'a', '\\'
 */
static void scan_char() {
  int value = 0;
  scan_expect_char('\'');
  if (scan_match_char('\'')) {
    scan_log_error_here("Char literal cannot be empty");
  } else if (*scan_state.stream == '\n') {
    scan_log_error_here("Char literal cannot contain newline");
  } else if (scan_match_char('\\')) {
    // Char is escaped
    if (*scan_state.stream == 'x') {
      // Hex escape sequence
      value = scan_hex_escape();
    } else {
      // Other escape sequence
      value = escape_to_char[(unsigned char)*scan_state.stream];
      if (value == 0 && *scan_state.stream != '0') {
        scan_log_error_here("Invalid char literal escape \\%c'",
                            *scan_state.stream);
      }
      ++scan_state.stream;
    }
  } else {
    value = *scan_state.stream;
    ++scan_state.stream;
  }

  // Closing single quote
  if (!scan_match_char('\'')) {
    scan_log_error_here("Expected closing char quote, got '%c'",
                        *scan_state.stream);
  }

  // Set token
  scan_state.token.token.type = TOKEN_INT;
  scan_state.token.token.int_val = value;
  scan_state.token.token.modifier = MOD_CHAR;
}

// TODO I don't quite understand the logic of this
// TODO A lot of testcases
static void scan_str_multi_line(char **str) {
  while (*scan_state.stream != 0) {
    // Skip empty strings
    if (scan_state.stream[0] == '"' && scan_state.stream[1] == '"' &&
        scan_state.stream[2] == '"') {
      scan_state.stream += 3;
    }

    // Push to buffer
    if (*scan_state.stream != '\r') {
      buffer_push(*str, *scan_state.stream);
    }

    // Process newlines
    if (*scan_state.stream != '\n') {
      ++scan_state.token.position.line;
    }
    ++scan_state.stream;
  }

  // String not closed
  if (*scan_state.stream == 0) {
    // TODO How do we know its multiline?
    // It might be just a multi-part string?
    scan_log_error_here(
        "Unexpected end of file within multi-line string literal");
  }
  scan_state.token.token.modifier = MOD_MULTILINE;
}

static void scan_str_single_line(char **str) {
  // Single line string
  while (scan_state.stream != 0 && *scan_state.stream != '"') {
    char value = *scan_state.stream;
    if (value == '\n') {
      scan_log_error_here("String literal cannot contain newline");
      break;
    } else if (scan_match_char('\\')) {
      // Escape sequence in string
      if (*scan_state.stream == 'x') {
        value = scan_hex_escape();
      } else {
        value = escape_to_char[(unsigned char)*scan_state.stream];
        if (value == 0 && *scan_state.stream != '0') {
          scan_log_error_here("Invalid string literal escape '\\%c'",
                              *scan_state.stream);
        }
        ++scan_state.stream;
      }
    } else {
      // Current stream char already stored in value
      ++scan_state.stream;
    }

    buffer_push(*str, value);
  }

  // Either string is closed or end of stream
  if (scan_state.stream == 0) {
    scan_log_error_here("Unexpected end of file within string literal");
  } else {
    ++scan_state.stream;
  }
}

/**
 * Scan string literal
 */
static void scan_str() {
  scan_expect_char('"');
  // String content buffer
  char *str = NULL;

  // Either single or multiline string
  if (scan_state.stream[0] == '"' && scan_state.stream[1] == '"') {
    // Multiple empty string literals
    scan_state.stream += 2;
    scan_str_multi_line(&str);
  } else {
    scan_str_single_line(&str);
  }

  // End string and set token
  buffer_push(str, 0);
  scan_state.token.token.type = TOKEN_STR;
  scan_state.token.token.str_val = str;
}

void scan_init(const char *name, const char *stream) {
  scan_state.stream = stream;
  scan_state.line_start = stream;

  // Default position name
  scan_state.token.position.name = name != NULL ? name : "<string>";
  scan_state.token.position.line = 1;

  // Scan first token from stream
  scan_next();
}

static void scan_skip_spaces() {
  while (isspace(*scan_state.stream)) {
    if (*scan_state.stream == '\n') {
      scan_state.line_start = scan_state.stream;
      ++scan_state.token.position.line;
    }
    ++scan_state.stream;
  }
}

static void scan_numeric() {
  assert(isdigit(*scan_state.stream));
  const char *stream = scan_state.stream;

  // Scanning either in or float
  // Do look-ahead until we know which
  while (isdigit((int)*stream)) {
    ++stream;
  }

  if (*stream == '.' || tolower(*stream) == 'e') {
    scan_float();
  } else {
    scan_int();
  }
}

static void scan_comment_single_line() {
  // Just read until end of line
  char *buffer = NULL;
  while (*scan_state.stream != 0) {
    if (scan_match_char('\n')) {
      break;
    }
    buffer_push(buffer, *scan_state.stream);
    ++scan_state.stream;
  }

  buffer_push(buffer, 0);
  scan_state.token.token.comment_text = buffer;
  scan_state.token.token.type = TOKEN_COMMENT;
}

static void scan_comment_multi_line() {
  char *buffer = NULL;
  while (*scan_state.stream != 0) {
    if (scan_state.stream[0] == '*' && scan_state.stream[1] == '/') {
      scan_state.stream += 2;
      break;
    } else if (*scan_state.stream == '\n') {
      ++scan_state.token.position.line;
    }
    buffer_push(buffer, *scan_state.stream);
    ++scan_state.stream;
  }
  buffer_push(buffer, 0);
  scan_state.token.token.comment_text = buffer;
  scan_state.token.token.type = TOKEN_COMMENT;
  scan_state.token.token.modifier = MOD_MULTILINE;
}

// Helper macros
#define CASE1(value, token_type)                                               \
  case value:                                                                  \
    scan_state.token.token.type = token_type;                                  \
    ++scan_state.stream;                                                       \
    break;

#define CASE2(value1, token_type1, value2, token_type2)                        \
  case value1:                                                                 \
    scan_state.token.token.type = token_type1;                                 \
    ++scan_state.stream;                                                       \
    if (*scan_state.stream == value2) {                                        \
      scan_state.token.token.type = token_type2;                               \
      ++scan_state.stream;                                                     \
    }                                                                          \
    break;

#define CASE3(value1, token_type1, value2, token_type2, value3, token_type3)   \
  case value1:                                                                 \
    scan_state.token.token.type = token_type1;                                 \
    ++scan_state.stream;                                                       \
    if (*scan_state.stream == value2) {                                        \
      scan_state.token.token.type = token_type2;                               \
      ++scan_state.stream;                                                     \
    } else if (*scan_state.stream == value3) {                                 \
      scan_state.token.token.type = token_type3;                               \
      ++scan_state.stream;                                                     \
    }                                                                          \
    break;

void scan_next() {
  bool repeat;
  do {
    repeat = false;
    scan_state.token.token.modifier = MOD_NONE;
    scan_state.token.token.suffix = SUFFIX_NONE;

    // Remove leading spaces, newlines, etc.
    scan_skip_spaces();
    scan_state.token.start = scan_state.stream;

    switch (*scan_state.stream) {
    case '\'':
      scan_char();
      break;

    case '"':
      scan_str();
      break;

    case '.':
      if (isdigit(scan_state.stream[1])) {
        scan_float();
      } else if (scan_state.stream[1] == '.' && scan_state.stream[2] == '.') {
        scan_state.token.token.type = TOKEN_ELLIPSIS;
        scan_state.stream += 3;
      } else {
        scan_state.token.token.type = TOKEN_DOT;
        ++scan_state.stream;
      }
      break;

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      scan_numeric();
      break;

    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case '_':
      // Identifier or keyword
      while (isalnum(*scan_state.stream) || *scan_state.stream == '_') {
        ++scan_state.stream;
      }
      scan_state.token.token.name =
          intern_string_range(scan_state.token.start, scan_state.stream);
      scan_state.token.token.type =
          keywords_is_keyword(scan_state.token.token.name) ? TOKEN_KEYWORD
                                                           : TOKEN_NAME;
      break;

    case '<':
      scan_state.token.token.type = TOKEN_LT;
      ++scan_state.stream;
      if (scan_match_char('<')) {
        scan_state.token.token.type = TOKEN_LSHIFT;
        if (scan_match_char('=')) {
          scan_state.token.token.type = TOKEN_LSHIFT_ASSIGN;
        }
      } else if (scan_match_char('=')) {
        scan_state.token.token.type = TOKEN_LTEQ;
      }
      break;

    case '>':
      scan_state.token.token.type = TOKEN_GT;
      ++scan_state.stream;
      if (scan_match_char('>')) {
        scan_state.token.token.type = TOKEN_RSHIFT;
        if (scan_match_char('=')) {
          scan_state.token.token.type = TOKEN_RSHIFT_ASSIGN;
        }
      } else if (scan_match_char('=')) {
        scan_state.token.token.type = TOKEN_GTEQ;
      }
      break;

    case '/':
      scan_state.token.token.type = TOKEN_DIV;
      ++scan_state.stream;
      if (scan_match_char('=')) {
        scan_state.token.token.type = TOKEN_DIV_ASSIGN;
      } else if (scan_match_char('/')) {
        // Single line comment
        scan_comment_single_line();
      } else if (scan_match_char('*')) {
        scan_comment_multi_line();
      }
      break;

    case '\0':
      // End of stream reached, do not move stream pointer
      scan_state.token.token.type = TOKEN_EOF;
      break;

      CASE1('(', TOKEN_LPAREN)
      CASE1(')', TOKEN_RPAREN)
      CASE1('{', TOKEN_LBRACE)
      CASE1('}', TOKEN_RBRACE)
      CASE1('[', TOKEN_LBRACKET)
      CASE1(']', TOKEN_RBRACKET)
      CASE1(',', TOKEN_COMMA)
      CASE1('@', TOKEN_AT)
      CASE1('#', TOKEN_POUND)
      CASE1('?', TOKEN_QUESTION)
      CASE1(';', TOKEN_SEMICOLON)
      CASE1('~', TOKEN_NEG) // TODO No negate assign "~="?
      CASE2('!', TOKEN_NOT, '=', TOKEN_NOTEQ)
      CASE2(':', TOKEN_COLON, '=', TOKEN_COLON_ASSIGN)
      CASE2('=', TOKEN_ASSIGN, '=', TOKEN_EQ)
      CASE2('^', TOKEN_XOR, '=', TOKEN_XOR_ASSIGN)
      CASE2('*', TOKEN_MUL, '=', TOKEN_MUL_ASSIGN)
      CASE2('%', TOKEN_MOD, '=', TOKEN_MOD_ASSIGN)
      CASE3('+', TOKEN_ADD, '=', TOKEN_ADD_ASSIGN, '+', TOKEN_INC)
      CASE3('-', TOKEN_SUB, '=', TOKEN_SUB_ASSIGN, '-', TOKEN_DEC)
      CASE3('&', TOKEN_AND, '=', TOKEN_AND_ASSIGN, '&', TOKEN_AND_AND)
      CASE3('|', TOKEN_OR, '=', TOKEN_OR_ASSIGN, '|', TOKEN_OR_OR)

    default:
      scan_log_error_here("Invalid '%c' token, skipping", *scan_state.stream);
      ++scan_state.stream;
      repeat = true;
    }
  } while (repeat);

  // Finalize token
  scan_state.token.end = scan_state.stream;
}

const scan_token_t *scan_get_token() { return &scan_state.token; }

bool scan_is_type(token_type_t type) {
  return scan_state.token.token.type == type;
}

bool scan_is_eof() { return scan_is_type(TOKEN_EOF); }

bool scan_is_name(const char *name) {
  return scan_is_type(TOKEN_NAME) && scan_state.token.token.name == name;
}

bool scan_is_keyword(const char *keyword) {
  return scan_is_type(TOKEN_KEYWORD) && scan_state.token.token.name == keyword;
}

bool scan_match_keyword(const char *keyword) {
  if (!scan_is_keyword(keyword)) {
    return false;
  }
  scan_next();
  return true;
}

bool scan_match_type(token_type_t type) {
  if (!scan_is_type(type)) {
    return false;
  }
  scan_next();
  return true;
}

void scan_expect_type(token_type_t type) {
  if (!scan_match_type(type)) {
    scan_log_fatal_here("Expected token %s, got %s", token_type_name(type),
                        scan_token_info());
  }
}