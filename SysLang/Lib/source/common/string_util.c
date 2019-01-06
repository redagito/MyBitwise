#include "syl/common/string_util.h"

#include <ctype.h>
#include <string.h>

bool string_islower(const char *str) {
  while (*str != 0) {
    if (isalpha(*str) && !islower(*str)) {
      return false;
    }
    ++str;
  }
  return true;
}

int string_search_array(const char *str, const char **str_array,
                        size_t array_size, int default_value) {
  for (int i = 0; i < array_size; ++i) {
    if (strcmp(str_array[i], str) == 0) {
      return i;
    }
  }
  return default_value;
}