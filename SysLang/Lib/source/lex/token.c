#include "syl/lex/token.h"

static const char *token_suffix_names[] = {
    [SUFFIX_NONE] = "",  [SUFFIX_D] = "d",   [SUFFIX_U] = "u",
    [SUFFIX_L] = "l",    [SUFFIX_UL] = "ul", [SUFFIX_LL] = "ll",
    [SUFFIX_ULL] = "ull"};

static const char *token_type_names[] = {
    [TOKEN_EOF] = "EOF",
    [TOKEN_COLON] = ":",
    [TOKEN_LPAREN] = "(",
    [TOKEN_RPAREN] = ")",
    [TOKEN_LBRACE] = "{",
    [TOKEN_RBRACE] = "}",
    [TOKEN_LBRACKET] = "[",
    [TOKEN_RBRACKET] = "]",
    [TOKEN_COMMA] = ",",
    [TOKEN_DOT] = ".",
    [TOKEN_AT] = "@",
    [TOKEN_POUND] = "#",
    [TOKEN_ELLIPSIS] = "...",
    [TOKEN_QUESTION] = "?",
    [TOKEN_SEMICOLON] = ";",
    [TOKEN_KEYWORD] = "keyword",
    [TOKEN_INT] = "int",
    [TOKEN_FLOAT] = "float",
    [TOKEN_STR] = "string",
    [TOKEN_NAME] = "name",
    [TOKEN_NEG] = "~",
    [TOKEN_NOT] = "!",
    [TOKEN_MUL] = "*",
    [TOKEN_DIV] = "/",
    [TOKEN_MOD] = "%",
    [TOKEN_AND] = "&",
    [TOKEN_LSHIFT] = "<<",
    [TOKEN_RSHIFT] = ">>",
    [TOKEN_ADD] = "+",
    [TOKEN_SUB] = "-",
    [TOKEN_OR] = "|",
    [TOKEN_XOR] = "^",
    [TOKEN_EQ] = "==",
    [TOKEN_NOTEQ] = "!=",
    [TOKEN_LT] = "<",
    [TOKEN_GT] = ">",
    [TOKEN_LTEQ] = "<=",
    [TOKEN_GTEQ] = ">=",
    [TOKEN_AND_AND] = "&&",
    [TOKEN_OR_OR] = "||",
    [TOKEN_ASSIGN] = "=",
    [TOKEN_ADD_ASSIGN] = "+=",
    [TOKEN_SUB_ASSIGN] = "-=",
    [TOKEN_OR_ASSIGN] = "|=",
    [TOKEN_AND_ASSIGN] = "&=",
    [TOKEN_XOR_ASSIGN] = "^=",
    [TOKEN_MUL_ASSIGN] = "*=",
    [TOKEN_DIV_ASSIGN] = "/=",
    [TOKEN_MOD_ASSIGN] = "%=",
    [TOKEN_LSHIFT_ASSIGN] = "<<=",
    [TOKEN_RSHIFT_ASSIGN] = ">>=",
    [TOKEN_INC] = "++",
    [TOKEN_DEC] = "--",
    [TOKEN_COLON_ASSIGN] = ":=",
};

token_type_t assign_token_to_binary_token[NUM_TOKEN_TYPES] = {
    [TOKEN_ADD_ASSIGN] = TOKEN_ADD,       [TOKEN_SUB_ASSIGN] = TOKEN_SUB,
    [TOKEN_OR_ASSIGN] = TOKEN_OR,         [TOKEN_AND_ASSIGN] = TOKEN_AND,
    [TOKEN_XOR_ASSIGN] = TOKEN_XOR,       [TOKEN_LSHIFT_ASSIGN] = TOKEN_LSHIFT,
    [TOKEN_RSHIFT_ASSIGN] = TOKEN_RSHIFT, [TOKEN_MUL_ASSIGN] = TOKEN_MUL,
    [TOKEN_DIV_ASSIGN] = TOKEN_DIV,       [TOKEN_MOD_ASSIGN] = TOKEN_MOD};

const char *token_type_name(token_type_t token_type) {
  if (token_type >= sizeof(token_type_names) / sizeof(*token_type_names)) {
    // Out of range
    return "<unknown>";
  }
  return token_type_names[token_type];
}

const char *token_suffix_name(token_suffix_t token_suffix) {
  if (token_suffix >=
      sizeof(token_suffix_names) / sizeof(*token_suffix_names)) {
    // Out of range
    return "<unknown>";
  }
  return token_suffix_names[token_suffix];
}

const char *token_info(token_t *token) {
  if (token->type == TOKEN_NAME || token->type == TOKEN_KEYWORD) {
    return token->name;
  }
  return token_type_name(token->type);
}