#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * Stretchy buffer
 */

typedef struct buffer_header_t {
  size_t length;
  size_t capacity;
  char buffer[];
} buffer_header_t;

// Internal API
#define buffer_header_internal(buf)                                            \
  ((buffer_header_t *)((char *)(buf)-offsetof(buffer_header_t, buffer)))

// Access to buffer internals
#define buffer_length(buf) ((buf) ? buffer_header_internal(buf)->length : 0)
#define buffer_capacity(buf) ((buf) ? buffer_header_internal(buf)->capacity : 0)
// Pointer to end of buffer
#define buffer_end(buf) ((buf) + buffer_length(buf))
// Total size of buffer
#define buffer_sizeof(buf) ((buf) ? buffer_length(buf) * sizeof(*buf) : 0)

// Free buffer memory
#define buffer_free(buf)                                                       \
  ((buf) ? (free(buffer_header_internal(buf)), (buf) = NULL) : 0)
// Grow buffer to fit total n amount of elements
#define buffer_fit(buf, n)                                                     \
  ((n) <= buffer_capacity(buf)                                                 \
       ? 0                                                                     \
       : ((buf) = buffer_grow_internal((buf), (n), sizeof(*(buf)))))
// Push new element into buffer
#define buffer_push(buf, ...)                                                  \
  (buffer_fit((buf), 1 + buffer_length(buf)),                                  \
   (buf)[buffer_header_internal(buf)->length++] = (__VA_ARGS__))
// Put formatted string into the buffer
#define buffer_printf(buf, ...)                                                \
  ((buf) = buffer_printf_internal((buf), __VA_ARGS__))
// Clear buffer contents
#define buffer_clear(buf) ((buf) ? buffer_header_internal(buf)->length = 0 : 0)

/**
 * Internal API
 * Grows buffer to capacity required to store at least new_length elements
 * with element_size.
 */
void *buffer_grow_internal(const void *buffer, size_t new_length,
                           size_t element_size);

/**
 * Internal API
 * Concatenates formatted string to buffer.
 * Grows buffer if required.
 */
char *buffer_printf_internal(char *buffer, const char *fmt, ...);