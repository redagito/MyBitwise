#include "syl/target.h"

#include <string.h>

#include "syl/common/string_util.h"
#include "syl/path.h"

static const char *arch_names[ARCH_COUNT] = {
    [ARCH_X86] = "x86",
    [ARCH_X64] = "x64",
};

static const char *os_names[OS_COUNT] = {[OS_WIN32] = "win32",
                                         [OS_LINUX] = "linux"};

os_t os_from_string(const char *name) {
  return string_search_array(name, os_names, OS_COUNT, OS_INVALID);
}

const char *os_to_string(os_t os) {
  if (os < 0 || os >= OS_COUNT) {
    return NULL;
  }
  return os_names[os];
}

const char **target_os_names() { return os_names; }

arch_t arch_from_string(const char *name) {
  return string_search_array(name, arch_names, ARCH_COUNT, ARCH_INVALID);
}

const char *arch_to_string(arch_t arch) {
  if (arch < 0 || arch >= ARCH_COUNT) {
    return NULL;
  }
  return arch_names[arch];
}

const char **target_arch_names() { return arch_names; }

static const char *target_get_filename_suffix(char suffix[MAX_PATH],
                                              const char *start,
                                              const char *end) {
  // Start at end
  const char *curr = end;
  // Search from end until first '_'
  while (curr != start && curr[-1] != '_') {
    --curr;
  }

  if (curr == start) {
    // No suffix
    suffix[0] = 0;
    return curr;
  }

  // Copy suffix
  memcpy(suffix, curr, end - curr);
  suffix[end - curr] = 0;
  // Step past '_'
  --curr;
  return curr;
}

bool target_is_excluded_filename(const char *name, os_t os, arch_t arch) {
  const char *end = name + strlen(name);
  const char *ptr = end;

  // Optional suffix for file names
  char suffix1[MAX_PATH];
  char suffix2[MAX_PATH];

  // Extract first suffix starting at end
  const char *curr = target_get_filename_suffix(suffix1, name, end);
  if (curr == name) {
    // No suffix
    return false;
  }
  // Extract second suffix
  curr = target_get_filename_suffix(suffix2, name, curr);

  // Resolve suffixes
  os_t os1 = os_from_string(suffix1);
  arch_t arch1 = arch_from_string(suffix1);
  os_t os2 = os_from_string(suffix2);
  arch_t arch2 = arch_from_string(suffix2);

  // Suffix 1 must be valid for suffix 2 to be considered
  if (os1 != OS_INVALID && arch2 != ARCH_INVALID) {
    return os1 != os || arch2 != arch;
  } else if (arch1 != ARCH_INVALID && os2 != OS_INVALID) {
    return os2 != os || arch1 != arch;
  } else if (os1 != OS_INVALID) {
    return os1 != os;
  } else if (arch1 != ARCH_INVALID) {
    return arch1 != arch;
  }
  return false;
}