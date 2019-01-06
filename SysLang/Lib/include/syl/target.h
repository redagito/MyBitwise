#pragma once

#include <stdbool.h>

/**
 * Supported targets for code generation
 */

/**
 * Supported operating systems
 */
typedef enum os_t {
  OS_INVALID = -1,
  OS_WIN32,
  OS_LINUX,

  OS_COUNT
} os_t;

/**
 * Supported architectures
 */
typedef enum arch_t {
  ARCH_INVALID = -1,
  ARCH_X86,
  ARCH_X64,

  ARCH_COUNT
} arch_t;

os_t os_from_string(const char *name);
const char *os_to_string(os_t os);
const char **target_os_names();

arch_t arch_from_string(const char *name);
const char *arch_to_string(arch_t arch);
const char **target_arch_names();

/**
 * Whether the provided name should be excluded for the target
 * File names may have up to 2 optional suffixes with os and arch name.
 * If any of these suffixes do not match the target os and arch, the file is
 * excluded. Example: "foobar_win32_x86", "barfoo_x64_linux", "foo_win32" name -
 * Name of a file without extension os - target operating system arch - target
 * architecture
 */
bool target_is_excluded_filename(const char *name, os_t os, arch_t arch);