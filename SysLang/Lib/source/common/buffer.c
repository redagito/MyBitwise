#include "syl/common/buffer.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#include "syl/common/log.h"
#include "syl/common/memory.h"

void *buffer_grow_internal(const void *buffer, size_t new_length,
                           size_t element_size) {
  // Buffer capacity below allowed capacity before grow
  assert(buffer_capacity(buffer) <= (SIZE_MAX - 1) / 2);

  // New buffer capacity, grow by at least 16 elements
  size_t new_capacity = max(2 * buffer_capacity(buffer), max(new_length, 16));

  // New capacity satisfies requested length
  assert(new_length <= new_capacity);
  assert(new_capacity <=
         (SIZE_MAX - offsetof(buffer_header_t, buffer)) / element_size);

  // New total size for the buffer including header
  size_t new_size =
      offsetof(buffer_header_t, buffer) + new_capacity * element_size;

  // Create new buffer or reallocate existing one
  buffer_header_t *new_header;
  if (buffer != NULL) {
    new_header = xrealloc(buffer_header_internal(buffer), new_size);
  } else {
    new_header = xmalloc(new_size);
    new_header->length = 0;
  }
  new_header->capacity = new_capacity;

  return new_header->buffer;
}

static size_t buffer_vsnprintf(char *buffer, const char *fmt, va_list args) {
  // Available buffer capacity
  size_t capacity = buffer_capacity(buffer) - buffer_length(buffer);

  // Attempt to concatenate onto buffer
  size_t length = vsnprintf(buffer_end(buffer), capacity, fmt, args);
  if (length <= 0) {
    log_fatal("Failed to format string: Encoding error occured");
  }

  // Length including null-terminator
  return length + 1;
}

char *buffer_printf_internal(char *buffer, const char *fmt, ...) {
  // Available buffer capacity
  size_t capacity = buffer_capacity(buffer) - buffer_length(buffer);

  // Attempt to concatenate onto buffer
  va_list args;
  va_start(args, fmt);
  size_t length = buffer_vsnprintf(buffer, fmt, args);
  va_end(args);

  if (length > capacity) {
    // Length required is higher than available capacity
    // Grow buffer and concatenate again
    buffer_fit(buffer, length + buffer_length(buffer));
    capacity = buffer_capacity(buffer) - buffer_length(buffer);

    va_list args;
    va_start(args, fmt);
    size_t length = buffer_vsnprintf(buffer, fmt, args);
    va_end(args);

    // Should fit now
    assert(length <= capacity);
  }

  // Add length of formatted string without null terminator
  buffer_header_internal(buffer)->length += length - 1;
  return buffer;
}