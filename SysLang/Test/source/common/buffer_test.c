#include "testy.h"

#include <string.h>

#include <syl/common/buffer.h>

TESTY_TEST(buffer) {
  // Empty buffer
  int *buffer = NULL;
  TESTY_ASSERT(buffer == NULL);
  TESTY_ASSERT(buffer_length(buffer) == 0);
  TESTY_ASSERT(buffer_capacity(buffer) == 0);
  TESTY_ASSERT(buffer_end(buffer) == 0);
  TESTY_ASSERT(buffer_sizeof(buffer) == 0);

  // Fill buffer
  int n = 1024;
  for (int i = 0; i < n; ++i) {
    buffer_push(buffer, i);
    TESTY_ASSERT(buffer != NULL);
    TESTY_ASSERT(buffer_length(buffer) == i + 1);
    TESTY_ASSERT(buffer_capacity(buffer) >= i + 1);
  }
  TESTY_ASSERT(buffer != NULL);
  TESTY_ASSERT(buffer_length(buffer) == n);
  TESTY_ASSERT(buffer_capacity(buffer) >= n);
  TESTY_ASSERT(buffer_end(buffer) != NULL);
  TESTY_ASSERT(buffer_sizeof(buffer) == n * sizeof(int));

  // Free buffer
  buffer_free(buffer);
  TESTY_ASSERT(buffer == NULL);
  TESTY_ASSERT(buffer_length(buffer) == 0);
  TESTY_ASSERT(buffer_capacity(buffer) == 0);
  TESTY_ASSERT(buffer_end(buffer) == 0);
  TESTY_ASSERT(buffer_sizeof(buffer) == 0);

  // Empty char buffer
  char *str = NULL;
  TESTY_ASSERT(str == NULL);
  TESTY_ASSERT(buffer_length(str) == 0);
  TESTY_ASSERT(buffer_capacity(str) == 0);
  TESTY_ASSERT(buffer_end(str) == 0);
  TESTY_ASSERT(buffer_sizeof(str) == 0);

  // Print formatted string to buffer
  buffer_printf(str, "One: %d\n", 1);
  TESTY_ASSERT(str != NULL);
  TESTY_ASSERT(strcmp(str, "One: 1\n") == 0);

  buffer_printf(str, "Hex: 0x%x\n", 0x12345678);
  TESTY_ASSERT(str != NULL);
  TESTY_ASSERT(strcmp(str, "One: 1\nHex: 0x12345678\n") == 0);

  buffer_clear(str);
  buffer_printf(str, "One: %d\n", 1);
  TESTY_ASSERT(str != NULL);
  TESTY_ASSERT(strcmp(str, "One: 1\n") == 0);
}