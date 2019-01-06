#pragma once

/**
 * Reachability
 */
typedef enum reachable_t {
  REACHABLE_NONE,
  REACHABLE_NATURAL,
  REACHABLE_FORCED
} reachable_t;

typedef enum symbol_type_t {
  SYM_NONE,
  SYM_VAR,
  SYM_CONST,
  SYM_FUNC,
  SYM_TYPE,
  SYM_PACKAGE
} symbol_type_t;

typedef enum symbol_state_t {
  SYM_UNRESOLVED,
  SYM_RESOLVING,
  SYM_RESOLVED
} symbol_state_t;

typedef struct package_t package_t;
typedef struct declare_t declare_t;

typedef struct symbol_t {
  const char *name;
  package_t *home_package;
  symbol_type_t type;
  symbol_state_t state;
  reachable_t reachable;
  declare_t *declare;
} symbol_t;