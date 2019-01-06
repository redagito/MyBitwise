#include "syl/resolve/package.h"

#include <string.h>

#include "syl/ast/ast.h"
#include "syl/common/buffer.h"
#include "syl/common/file.h"
#include "syl/common/intern.h"
#include "syl/common/log.h"
#include "syl/common/memory.h"
#include "syl/dir.h"
#include "syl/lex/scan.h"
#include "syl/parse/parse.h"
#include "syl/path.h"

// Search paths for package lookup
enum { MAX_SEARCH_PATHS = 256 };
static const char *package_static_search_paths[MAX_SEARCH_PATHS];
static const char **package_search_paths = package_static_search_paths;
static int package_search_path_count = 0;

// Loaded packages
// Maps interned package path (not full_path) to package
static map_t package_map;
// List of all loaded packages
static package_t **package_list;

void package_add_search_path(const char *path, bool verbose) {
  if (verbose) {
    log_info("Adding package search path %s", path);
  }
  package_search_paths[package_search_path_count++] = intern_string(path);
}

void package_add_search_path_range(const char *start, const char *end,
                                   bool verbose) {
  char path[MAX_PATH];
  size_t length = min(end - start, MAX_PATH - 1);
  // Copy without delimiter
  memcpy(path, start, length);
  path[length] = 0;
  package_add_search_path(path, verbose);
}

/**
 * Whether a package can be found in the path
 * search_path - Base search path for packages
 * package_path - Path relative to search path
 */
static bool package_in_path(const char *search_path, const char *package_path) {
  // Build full path
  char path[MAX_PATH];
  path_copy(path, search_path);
  path_join(path, package_path);

  // Check for source files in path
  dir_list_iter_t iter;
  for (dir_list(&iter, path); iter.valid; dir_list_next(&iter)) {
    const char *ext = path_ext(iter.name);
    if (!iter.is_dir && ext != iter.name && strcmp(ext, "syl") == 0) {
      // Found file with language extension
      dir_list_free(&iter);
      return true;
    }
  }

  // No source files found at path
  return false;
}

/**
 * Find and copy full path for the package
 * Searches all package search paths for the package
 * Returns true on success and false on failure
 */
static bool package_copy_full_path(char full_path[MAX_PATH],
                                   const char *package_path) {
  for (int i = 0; i < package_search_path_count; ++i) {
    // Check if search path is valid for the package
    // TODO What if multiple search paths hold the same package name?
    //      Check all search paths and issue error if multiple matches are
    //      found?
    if (package_in_path(package_search_paths[i], package_path)) {
      path_copy(full_path, package_search_paths[i]);
      path_join(full_path, package_path);
      return true;
    }
  }

  // Package not found
  return false;
}

static void package_add(package_t *package, bool verbose) {
  package_t *old_package = map_get(&package_map, package->path);
  if (old_package == package) {
    // Same package already exists
    return;
  } else if (old_package != NULL) {
    // Different package with same path
    log_fatal("Package %s with path %s already exists with path %s",
              package->path, package->full_path, old_package->full_path);
  }

  map_put(&package_map, package->path, package);
  buffer_push(package_list, package);
  if (verbose) {
    log_info("Add package %s at path %s", package->path, package->full_path);
  }
}

static bool package_parse(package_t *package, os_t os, arch_t arch,
                          bool verbose) {
  declare_t **declares = NULL;
  dir_list_iter_t iter;
  for (dir_list(&iter, package->full_path); iter.valid; dir_list_next(&iter)) {
    // Source files may not start with '_' or '.'
    if (iter.is_dir || iter.name[0] == '_' || iter.name[0] == '.') {
      // Skip
      continue;
    }

    // Create writable copy
    char file_name[MAX_PATH];
    path_copy(file_name, iter.name);
    char *ext = path_ext(file_name);
    if (ext == file_name || strcmp(ext, "syl") != 0) {
      // Not a source file, skip
      continue;
    }
    // Remove extension from copy
    ext[-1] = 0;
    if (target_is_excluded_filename(file_name, os, arch)) {
      continue;
    }

    // Read source file into memory
    char path[MAX_PATH];
    path_copy(path, iter.base);
    path_join(path, iter.name);
    path_absolute(path);
    const char *code = file_read(path);
    if (code == NULL) {
      log_fatal("Failed to read source file %s", path);
    }

    // Parse
    scan_init(intern_string(path), code);
    declares_t *file_declares = parse_declares();
  }

  return true;
}

static bool package_compile(package_t *package, os_t os, arch_t arch,
                            bool verbose) {
  if (!package_parse(package, os, arch, verbose)) {
    return false;
  }
  return true;
}

package_t *package_import(const char *package_path, os_t os, arch_t arch,
                          bool verbose) {
  // Interned path string can be used as unique key
  // TODO What if same package is loaded with different package paths?
  //      If thats even possible?
  package_path = intern_string(package_path);
  package_t *package = map_get(&package_map, package_path);

  if (package == NULL) {
    // Package not in map
    package = xcalloc(1, sizeof(package_t));
    package->path = package_path;
    if (verbose) {
      log_info("Importing package %s", package_path);
    }

    // Find package in search paths and set full path
    if (!package_copy_full_path(package->full_path, package_path)) {
      log_error("Unable to find package %s", package_path);
      free(package);
      return NULL;
    }

    // TODO Missing error check for package compilation here
    //      Can we compile first and then add the package?
    package_add(package, verbose);
    package_compile(package, os, arch, verbose);
    // TODO This is what it *should* look like
    /*
    if (!package_compile(package, verbose))
    {
            log_error("Failed to compile package %s", package_path);
            free(package);
            return NULL;
    }
    */
  }
  return package;
}