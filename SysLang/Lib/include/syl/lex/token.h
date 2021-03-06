#pragma once

/**
 * Lexer token
 */
typedef enum token_type_t {
  TOKEN_EOF,
  TOKEN_COLON,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_LBRACE,
  TOKEN_RBRACE,
  TOKEN_LBRACKET,
  TOKEN_RBRACKET,
  TOKEN_COMMA,
  TOKEN_DOT,
  TOKEN_AT,
  TOKEN_POUND,
  TOKEN_ELLIPSIS,
  TOKEN_QUESTION,
  TOKEN_SEMICOLON,
  TOKEN_KEYWORD,
  TOKEN_INT,
  TOKEN_FLOAT,
  TOKEN_STR,
  TOKEN_NAME, // TODO Change to TOKEN_IDENTIFIER
  TOKEN_NEG,  // Bitwise negation
  TOKEN_NOT,
  TOKEN_COMMENT,

  // Multiplicative precedence
  TOKEN_FIRST_MUL,
  TOKEN_MUL = TOKEN_FIRST_MUL,
  TOKEN_DIV,
  TOKEN_MOD,
  TOKEN_AND,
  TOKEN_LSHIFT,
  TOKEN_RSHIFT,
  TOKEN_LAST_MUL = TOKEN_RSHIFT,

  // Additive precedence
  TOKEN_FIRST_ADD,
  TOKEN_ADD = TOKEN_FIRST_ADD,
  TOKEN_SUB,
  TOKEN_XOR,
  TOKEN_OR, // Bitwise OR
  TOKEN_LAST_ADD = TOKEN_OR,

  // Comparative precedence
  TOKEN_FIRST_CMP,
  TOKEN_EQ = TOKEN_FIRST_CMP,
  TOKEN_NOTEQ,
  TOKEN_LT,
  TOKEN_GT,
  TOKEN_LTEQ,
  TOKEN_GTEQ,
  TOKEN_LAST_CMP = TOKEN_GTEQ,
  TOKEN_AND_AND,
  TOKEN_OR_OR, // Logical OR

  // Assignment operators
  TOKEN_FIRST_ASSIGN,
  TOKEN_ASSIGN = TOKEN_FIRST_ASSIGN,
  TOKEN_ADD_ASSIGN,
  TOKEN_SUB_ASSIGN,
  TOKEN_OR_ASSIGN,
  TOKEN_AND_ASSIGN,
  TOKEN_XOR_ASSIGN,
  TOKEN_LSHIFT_ASSIGN,
  TOKEN_RSHIFT_ASSIGN,
  TOKEN_MUL_ASSIGN,
  TOKEN_DIV_ASSIGN,
  TOKEN_MOD_ASSIGN,
  TOKEN_LAST_ASSIGN = TOKEN_MOD_ASSIGN,
  TOKEN_INC,
  TOKEN_DEC,
  TOKEN_COLON_ASSIGN,

  NUM_TOKEN_TYPES
} token_type_t;

typedef enum token_modifier_t {
  MOD_NONE,
  MOD_HEX,
  MOD_BIN,
  MOD_OCT,
  MOD_CHAR,
  MOD_MULTILINE
} token_modifier_t;

/**
 * Suffixes for numerical literals
 */
typedef enum token_suffix_t {
  SUFFIX_NONE,
  SUFFIX_D,  // double
  SUFFIX_U,  // unsigned
  SUFFIX_L,  // long
  SUFFIX_UL, // unsigned long
  SUFFIX_LL, // long long
  SUFFIX_ULL // unsigned long long
} token_suffix_t;

/**
 * Token
 */
typedef struct token_t {
  // Info
  token_type_t type;
  token_modifier_t modifier;
  token_suffix_t suffix;

  // Data
  union {
    // Set for numeric types
    unsigned long long int_val;
    double float_val;
    // Set for strings
    const char *str_val;
    // Set for identifiers
    const char *name;
    // Set for comments (single- or multiline)
    const char *comment_text;
  };
} token_t;

/**
 * String representation of the token type
 */
const char *token_type_name(token_type_t token_type);

/**
 * String representation of the token suffix
 */
const char *token_suffix_name(token_suffix_t token_suffix);

/**
 * Returns either name (for identifiers and keywords) or type of token
 */
const char *token_info(token_t *token);