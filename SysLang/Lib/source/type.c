#include "syl/type.h"

#include "syl/common/log.h"
#include "syl/common/map.h"
#include "syl/common/memory.h"

/**
 * Type metrics for supported operating systems and architecture combinations
 */

// Default metrics, same for all os/arch combinations
#define DEFAULT_TYPE_METRICS                                                   \
  [TYPE_BOOL] = {.size = 1, .align = 1},                                       \
  [TYPE_CHAR] = {.size = 1, .align = 1, .max = 0x7f, .sign = true},            \
  [TYPE_SCHAR] = {.size = 1, .align = 1, .max = 0x7f, .sign = true},           \
  [TYPE_UCHAR] = {.size = 1, .align = 1, .max = 0xff},                         \
  [TYPE_SHORT] = {.size = 2, .align = 2, .max = 0x7fff, .sign = true},         \
  [TYPE_USHORT] = {.size = 2, .align = 2, .max = 0xffff},                      \
  [TYPE_INT] = {.size = 4, .align = 4, .max = 0x7fffffff, .sign = true},       \
  [TYPE_UINT] = {.size = 4, .align = 4, .max = 0xffffffff},                    \
  [TYPE_LLONG] = {.size = 8,                                                   \
                  .align = 8,                                                  \
                  .max = 0x7fffffffffffffff,                                   \
                  .sign = true},                                               \
  [TYPE_ULLONG] = {.size = 8, .align = 8, .max = 0xffffffffffffffff},          \
  [TYPE_FLOAT] = {.size = 4, .align = 4},                                      \
  [TYPE_DOUBLE] = {.size = 8, .align = 8}

static type_metrics_t win32_x86_metrics[TYPE_COUNT] = {
    DEFAULT_TYPE_METRICS, [TYPE_PTR] = {.size = 4, .align = 4},
    [TYPE_LONG] = {.size = 4, .align = 4, .max = 0x7fffffff, .sign = true},
    [TYPE_ULONG] = {.size = 4, .align = 4, .max = 0x7fffffff, .sign = true}};

static type_metrics_t win32_x64_metrics[TYPE_COUNT] = {
    DEFAULT_TYPE_METRICS, [TYPE_PTR] = {.size = 8, .align = 8},
    [TYPE_LONG] = {.size = 4, .align = 4, .max = 0x7fffffff, .sign = true},
    [TYPE_ULONG] = {.size = 4, .align = 4, .max = 0x7fffffff, .sign = true}};

static type_metrics_t ilp32_metrics[TYPE_COUNT] = {
    DEFAULT_TYPE_METRICS,
    [TYPE_PTR] = {.size = 4, .align = 4},
    [TYPE_LONG] = {.size = 4, .align = 4, .max = 0x7fffffff, .sign = true},
    [TYPE_ULONG] = {.size = 4, .align = 4, .max = 0x7fffffff, .sign = true},
};

static type_metrics_t lp64_metrics[TYPE_COUNT] = {
    DEFAULT_TYPE_METRICS,
    [TYPE_PTR] = {.size = 8, .align = 8},
    [TYPE_LONG] = {.size = 8,
                   .align = 8,
                   .max = 0x7fffffffffffffff,
                   .sign = true},
    [TYPE_ULONG] = {.size = 8,
                    .align = 8,
                    .max = 0xffffffffffffffff,
                    .sign = true},
};

// Currently active type metrics
static type_metrics_t *type_metrics = NULL;

// Builtin types with default settings
static type_t *type_void = &(type_t){.type = TYPE_VOID};
static type_t *type_bool = &(type_t){.type = TYPE_BOOL};

static type_t *type_char = &(type_t){.type = TYPE_CHAR};
static type_t *type_uchar = &(type_t){.type = TYPE_UCHAR};
static type_t *type_schar = &(type_t){.type = TYPE_SCHAR};

static type_t *type_short = &(type_t){.type = TYPE_SHORT};
static type_t *type_ushort = &(type_t){.type = TYPE_USHORT};

static type_t *type_int = &(type_t){.type = TYPE_INT};
static type_t *type_uint = &(type_t){.type = TYPE_UINT};

static type_t *type_long = &(type_t){.type = TYPE_LONG};
static type_t *type_ulong = &(type_t){.type = TYPE_ULONG};

static type_t *type_llong = &(type_t){.type = TYPE_LLONG};
static type_t *type_ullong = &(type_t){.type = TYPE_ULLONG};

static type_t *type_float = &(type_t){.type = TYPE_FLOAT};
static type_t *type_double = &(type_t){.type = TYPE_DOUBLE};

// Alias for builtin types dependent on architecture
// Set on init to one of the types above
static type_t *type_uintptr = NULL;
static type_t *type_usize = NULL;
static type_t *type_ssize = NULL;

// Typeid for next registered type
static typeid_t next_typeid = 1;

// Map with registered types by type id
static map_t type_id_map;

// Builtin type names
const char *type_names[TYPE_COUNT] = {
	[TYPE_VOID] = "void",
	[TYPE_BOOL] = "bool",
	[TYPE_CHAR] = "char",
	[TYPE_SCHAR] = "schar",
	[TYPE_UCHAR] = "uchar",
	[TYPE_SHORT] = "short",
	[TYPE_USHORT] = "ushort",
	[TYPE_INT] = "int",
	[TYPE_UINT] = "uint",
	[TYPE_LONG] = "long",
	[TYPE_ULONG] = "ulong",
	[TYPE_LLONG] = "llong",
	[TYPE_ULLONG] = "ullong",
	[TYPE_FLOAT] = "float",
	[TYPE_DOUBLE] = "double"
};

// Builtin type ranks
int type_ranks[TYPE_COUNT] = {
	[TYPE_BOOL] = 1,
	[TYPE_CHAR] = 2,
	[TYPE_SCHAR] = 2,
	[TYPE_UCHAR] = 2,
	[TYPE_SHORT] = 3,
	[TYPE_USHORT] = 3,
	[TYPE_INT] = 4,
	[TYPE_UINT] = 4,
	[TYPE_LONG] = 5,
	[TYPE_ULONG] = 5,
	[TYPE_LLONG] = 6,
	[TYPE_ULLONG] = 6
};

static void type_register(type_t *type) {
  map_put_from_uint64(&type_id_map, type->type_id, type);
}

type_t *type_alloc(type_type_t t)
{
	type_t* type = xcalloc(1, sizeof(type_t));
	type->type = t;
	type->type_id = next_typeid++;
	type_register(type);
	return type;
}

/**
 * Initialize single builtin type
 */
static void type_init_builtin(type_t *type) {
  type->type_id = next_typeid++;
  type_register(type);
  type->size = type_metrics[type->type].size;
  type->align = type_metrics[type->type].align;
}

/**
 * Initialize all builtin types
 */
static void type_init_builtins() {
  type_init_builtin(type_void);
  type_init_builtin(type_bool);
  type_init_builtin(type_char);
  type_init_builtin(type_uchar);
  type_init_builtin(type_schar);
  type_init_builtin(type_short);
  type_init_builtin(type_ushort);
  type_init_builtin(type_int);
  type_init_builtin(type_uint);
  type_init_builtin(type_long);
  type_init_builtin(type_ulong);
  type_init_builtin(type_llong);
  type_init_builtin(type_ullong);
  type_init_builtin(type_float);
  type_init_builtin(type_double);
}

static void type_init_metrics(os_t os, arch_t arch) {
  switch (os) {
  case OS_WIN32:
    switch (arch) {
    case ARCH_X86:
      type_metrics = win32_x86_metrics;
      break;

    case ARCH_X64:
      type_metrics = win32_x64_metrics;
      break;

    case ARCH_INVALID:
    default:
      break;
    }
    break;

  case OS_LINUX:
    switch (arch) {
    case ARCH_X86:
      type_metrics = ilp32_metrics;
      break;

    case ARCH_X64:
      type_metrics = lp64_metrics;
      break;

    case ARCH_INVALID:
    default:
      break;
    }
    break;

  case OS_INVALID:
  default:
    break;
  }

  if (type_metrics == NULL) {
    log_fatal("Unsupported OS/Arch combination: %s/%s", os_to_string(os),
              arch_to_string(arch));
  }

  if (type_metrics[TYPE_PTR].size == 4) {
    // 32 bit
    type_uintptr = type_uint;
    type_usize = type_uint;
    type_ssize = type_int;

  } else if (type_metrics[TYPE_PTR].size == 8) {
    // 64 bit
    type_uintptr = type_ullong;
    type_usize = type_ullong;
    type_ssize = type_llong;
  } else {
    log_fatal("Invalid or unsupported pointer type size: %i",
              type_metrics[TYPE_PTR].size);
  }
}

void type_init(os_t os, arch_t arch) {
  type_init_metrics(os, arch);
  type_init_builtins();
}

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

type_t *type_unsigned(type_t *type);

size_t type_sizeof(type_t *type);

size_t type_alignof(type_t *type);

type_t *type_ptr(type_t *base);

type_t *type_const(type_t *base);

type_t *type_unqualify(type_t *type);

type_t *type_array(type_t *base, size_t num_elements);

type_t *type_func(type_t **parameters, size_t num_parameters,
	type_t *return_type, bool has_varargs);

bool type_has_duplicate_fields(type_t *type);

void type_add_fields(type_field_t **fields, type_t *type, size_t offset);

void type_complete_struct(type_t *type, type_field_t *fields,
	size_t num_fields);

void type_complete_union(type_t *type, type_field_t *fields, size_t num_fields);

type_t *type_incomplete(symbol_t *sym);

type_t *type_enum(symbol_t *sym, type_t *base);

int type_aggregate_item_field_index(type_t* type, const char* name);

type_t* type_aggregate_item_field_type_by_index(type_t* type, int index);

type_t* type_aggregate_item_field_type_by_name(type_t* type, const char* name);