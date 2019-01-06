#pragma once

#include <stdint.h>

#include "syl/lex/scan.h"
#include "syl/lex/token.h"

/**
 * Structures which are part of the syntax tree for the language
 */
typedef struct expression_t expression_t;
typedef struct statement_t statement_t;
typedef struct declare_t declare_t;
typedef struct typespecifier_t typespecifier_t;

typedef struct note_argument_t {
  source_position_t pos;
  const char *name;
  expression_t *expr;
} note_argument_t;

typedef struct note_t {
  source_position_t pos;
  const char *name;
  note_argument_t *args;
  size_t num_args;
} note_t;

typedef struct notes_t {
  note_t *notes;
  size_t num_notes;
} notes_t;

typedef struct statement_list_t {
  source_position_t pos;
  statement_t **stmts;
  size_t num_stmts;
} statement_list_t;

typedef enum typespecifier_type_t {
  TYPESPEC_NONE,
  TYPESPEC_NAME,
  TYPESPEC_FUNC,
  TYPESPEC_ARRAY,
  TYPESPEC_PTR,
  TYPESPEC_CONST
} typespecifier_type_t;

struct typespecifier_t {
  typespecifier_type_t kind;
  source_position_t pos;
  typespecifier_t *base;
  union {
    struct {
      const char **names;
      size_t num_names;
    };
    struct {
      typespecifier_t **args;
      size_t num_args;
      bool has_varargs;
      typespecifier_t *ret;
    } func;
    expression_t *num_elems;
  };
};

typedef struct function_parameter_t {
  source_position_t pos;
  const char *name;
  typespecifier_t *type;
} function_parameter_t;

typedef enum aggregate_item_type_t {
  AGGREGATE_ITEM_NONE,
  AGGREGATE_ITEM_FIELD,
  AGGREGATE_ITEM_SUBAGGREGATE
} aggregate_item_type_t;

typedef struct aggregate_t aggregate_t;

typedef struct aggregate_item_t {
  source_position_t pos;
  aggregate_item_type_t kind;
  union {
    struct {
      const char **names;
      size_t num_names;
      typespecifier_t *type;
    };
    aggregate_t *subaggregate;
  };
} aggregate_item_t;

typedef struct enum_item_t {
  source_position_t pos;
  const char *name;
  expression_t *init;
} enum_item_t;

typedef struct import_item_t {
  const char *name;
  const char *rename;
} import_item_t;

typedef enum declare_type_t {
  DECL_NONE,
  DECL_ENUM,
  DECL_STRUCT,
  DECL_UNION,
  DECL_VAR,
  DECL_CONST,
  DECL_TYPEDEF,
  DECL_FUNC,
  DECL_NOTE,
  DECL_IMPORT
} declare_type_t;

typedef enum aggregate_type_t {
  AGGREGATE_NONE,
  AGGREGATE_STRUCT,
  AGGREGATE_UNION,
} aggregate_type_t;

typedef struct aggregate_t {
  source_position_t pos;
  aggregate_type_t kind;
  aggregate_item_t *items;
  size_t num_items;
} aggregate_t;

struct declare_t {
  declare_type_t kind;
  source_position_t pos;
  const char *name;
  notes_t notes;
  bool is_incomplete;
  union {
    note_t note;
    struct {
      typespecifier_t *type;
      enum_item_t *items;
      size_t num_items;
    } enum_decl;
    aggregate_t *aggregate;
    struct {
      function_parameter_t *params;
      size_t num_params;
      typespecifier_t *ret_type;
      bool has_varargs;
      statement_list_t block;
    } func;
    struct {
      typespecifier_t *type;
    } typedef_decl;
    struct {
      typespecifier_t *type;
      expression_t *expr;
    } var;
    struct {
      typespecifier_t *type;
      expression_t *expr;
    } const_decl;
    struct {
      bool is_relative;
      const char **names;
      size_t num_names;
      bool import_all;
      import_item_t *items;
      size_t num_items;
    } import;
  };
};

typedef struct declares_t {
  declare_t **decls;
  size_t num_decls;
} declares_t;

typedef enum expression_type_t {
  EXPR_NONE,
  EXPR_PAREN,
  EXPR_INT,
  EXPR_FLOAT,
  EXPR_STR,
  EXPR_NAME,
  EXPR_CAST,
  EXPR_CALL,
  EXPR_INDEX,
  EXPR_FIELD,
  EXPR_COMPOUND,
  EXPR_UNARY,
  EXPR_BINARY,
  EXPR_TERNARY,
  EXPR_MODIFY,
  EXPR_SIZEOF_EXPR,
  EXPR_SIZEOF_TYPE,
  EXPR_TYPEOF_EXPR,
  EXPR_TYPEOF_TYPE,
  EXPR_ALIGNOF_EXPR,
  EXPR_ALIGNOF_TYPE,
  EXPR_OFFSETOF
} expression_type_t;

typedef enum compound_field_type_t {
  FIELD_DEFAULT,
  FIELD_NAME,
  FIELD_INDEX
} compound_field_type_t;

typedef struct compound_field_t {
  compound_field_type_t kind;
  source_position_t pos;
  expression_t *init;
  union {
    const char *name;
    expression_t *index;
  };
} compound_field_t;

struct expression_t {
  expression_type_t kind;
  source_position_t pos;
  union {
    struct {
      expression_t *expr;
    } paren;
    struct {
      unsigned long long val;
      token_modifier_t mod;
      token_suffix_t suffix;
    } int_lit;
    struct {
      const char *start;
      const char *end;
      double val;
      token_suffix_t suffix;
    } float_lit;
    struct {
      const char *val;
      token_modifier_t mod;
    } str_lit;
    const char *name;
    expression_t *sizeof_expr;
    typespecifier_t *sizeof_type;
    expression_t *typeof_expr;
    typespecifier_t *typeof_type;
    expression_t *alignof_expr;
    typespecifier_t *alignof_type;
    struct {
      typespecifier_t *type;
      const char *name;
    } offsetof_field;
    struct {
      typespecifier_t *type;
      compound_field_t *fields;
      size_t num_fields;
    } compound;
    struct {
      typespecifier_t *type;
      expression_t *expr;
    } cast;
    struct {
      token_type_t op;
      bool post;
      expression_t *expr;
    } modify;
    struct {
      token_type_t op;
      expression_t *expr;
    } unary;
    struct {
      token_type_t op;
      expression_t *left;
      expression_t *right;
    } binary;
    struct {
      expression_t *cond;
      expression_t *then_expr;
      expression_t *else_expr;
    } ternary;
    struct {
      expression_t *expr;
      expression_t **args;
      size_t num_args;
    } call;
    struct {
      expression_t *expr;
      expression_t *index;
    } index;
    struct {
      expression_t *expr;
      const char *name;
    } field;
  };
};

typedef struct else_if_t {
  expression_t *cond;
  statement_list_t block;
} else_if_t;

typedef struct switch_case_pattern_t {
  expression_t *start;
  expression_t *end;
} switch_case_pattern_t;

typedef struct switch_case_t {
  switch_case_pattern_t *patterns;
  size_t num_patterns;
  bool is_default;
  statement_list_t block;
} switch_case_t;

typedef enum statement_type_t {
  STMT_NONE,
  STMT_DECL,
  STMT_RETURN,
  STMT_BREAK,
  STMT_CONTINUE,
  STMT_BLOCK,
  STMT_IF,
  STMT_WHILE,
  STMT_DO_WHILE,
  STMT_FOR,
  STMT_SWITCH,
  STMT_ASSIGN,
  STMT_INIT,
  STMT_EXPR,
  STMT_NOTE,
  STMT_LABEL,
  STMT_GOTO
} statement_type_t;

struct statement_t {
  statement_type_t kind;
  notes_t notes;
  source_position_t pos;
  union {
    note_t note;
    expression_t *expr;
    declare_t *decl;
    struct {
      statement_t *init;
      expression_t *cond;
      statement_list_t then_block;
      else_if_t *elseifs;
      size_t num_elseifs;
      statement_list_t else_block;
    } if_stmt;
    struct {
      expression_t *cond;
      statement_list_t block;
    } while_stmt;
    struct {
      statement_t *init;
      expression_t *cond;
      statement_t *next;
      statement_list_t block;
    } for_stmt;
    struct {
      expression_t *expr;
      switch_case_t *cases;
      size_t num_cases;
    } switch_stmt;
    statement_list_t block;
    struct {
      token_type_t op;
      expression_t *left;
      expression_t *right;
    } assign;
    struct {
      const char *name;
      typespecifier_t *type;
      expression_t *expr;
    } init;
    const char *label;
  };
};
