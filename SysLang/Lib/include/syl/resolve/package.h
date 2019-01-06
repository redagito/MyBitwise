#pragma once

#include <stdbool.h>

#include "syl/common/map.h"
#include "syl/path.h"
#include "syl/resolve/symbol.h"
#include "syl/target.h"

/**
 * Package functionality
 */

typedef struct package_t {
  const char *path;
  char full_path[MAX_PATH];
  declare_t **declares;
  size_t declare_count;
  map_t symbol_map;
  symbol_t **symbols;
  const char *external_name;
  bool always_reachable;
} package_t;

void package_add_search_path(const char *path, bool verbose);
void package_add_search_path_range(const char *start, const char *end,
                                   bool verbose);

/**
 * Import package by path
 */
package_t *package_import(const char *package_path, os_t os, arch_t arch,
                          bool verbose);