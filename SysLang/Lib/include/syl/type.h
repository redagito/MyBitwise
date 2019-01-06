#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "syl/target.h"

typedef enum type_type_t {
  TYPE_NONE,
  TYPE_INCOMPLETE,
  TYPE_COMPLETING,

  TYPE_VOID,

  TYPE_BOOL,
  TYPE_CHAR,
  TYPE_SCHAR,
  TYPE_UCHAR,
  TYPE_SHORT,
  TYPE_USHORT,
  TYPE_INT,
  TYPE_UINT,
  TYPE_LONG,
  TYPE_ULONG,
  TYPE_LLONG,
  TYPE_ULLONG,

  TYPE_ENUM,

  TYPE_FLOAT,
  TYPE_DOUBLE,

  TYPE_PTR,
  TYPE_FUNC,
  TYPE_ARRAY,

  TYPE_STRUCT,
  TYPE_UNION,

  TYPE_CONST,

  // Keep at end
  TYPE_COUNT
} type_type_t;

typedef struct type_t type_t;
typedef struct symbol_t symbol_t;

typedef struct type_field_t {
  const char *name;
  type_t *type;
  size_t offset;
} type_field_t;

typedef uint64_t typeid_t;

typedef struct type_t {
  type_type_t type;
  size_t size;
  size_t align;
  symbol_t *symbol;
  type_t *base;
  typeid_t type_id;
  bool non_modifiable;

  union {
    size_t element_count;
    // For aggregate type (struct or union)
    struct {
      type_field_t *fields;
      size_t field_count;
    } aggregate;

    // For function type
    struct {
      type_t **parameters;
      size_t parameter_count;
      bool has_varargs;
      type_t *return_type;
    } function;
  };
} type_t;

typedef struct type_metrics_t {
  size_t size;
  size_t align;
  bool sign;
  unsigned long long max;
} type_metrics_t;

/**
 * Initializes metrics and builtin types
 */
void type_init(os_t os, arch_t arch);

void type_complete(type_t *type);

type_t *type_get_by_id(typeid_t id);

bool type_is_ptr(type_t *type);
bool type_is_func(type_t *type);

/**
 * Is either pointer to data or function
 */
bool type_is_ptr_like(type_t *type);
bool type_is_const(type_t *type);
bool type_is_array(type_t *type);
bool type_is_incomplete_array(type_t *type);

bool type_is_integer(type_t *type);
bool type_is_floating(type_t *type);
bool type_is_arithmetic(type_t *type);
bool type_is_scalar(type_t *type);
bool type_is_aggregate(type_t *type);

bool type_is_signed(type_t *type);

int type_rank(type_t *type);

/**
 * Returns unsigned version of the given type.
 */
type_t *type_unsigned(type_t *type);

size_t type_sizeof(type_t *type);

size_t type_alignof(type_t *type);

/**
 * Returns pointer type to base type
 */
type_t *type_ptr(type_t *base);

/**
 * Returns const type of base type
 */
type_t *type_const(type_t *base);

/**
 * Returns type without const qualifier
 * which is either the type itself or its base type
 */
type_t *type_unqualify(type_t *type);

/**
 * Returns array type of base type
 */
type_t *type_array(type_t *base, size_t num_elements);

/**
 * Returns function type
 */
type_t *type_func(type_t **parameters, size_t num_parameters,
                  type_t *return_type, bool has_varargs);

/**
 * Aggregate type only
 */
bool type_has_duplicate_fields(type_t *type);

void type_add_fields(type_field_t **fields, type_t *type, size_t offset);

void type_complete_struct(type_t *type, type_field_t *fields,
                          size_t num_fields);

void type_complete_union(type_t *type, type_field_t *fields, size_t num_fields);

type_t *type_incomplete(symbol_t *sym);

type_t *type_enum(symbol_t *sym, type_t *base);

/**
* Returns index of field in aggregate by field name
*/
int type_aggregate_item_field_index(type_t* type, const char* name);

type_t* type_aggregate_item_field_type_by_index(type_t* type, int index);

type_t* type_aggregate_item_field_type_by_name(type_t* type, const char* name);