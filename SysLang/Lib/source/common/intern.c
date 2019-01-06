#include "syl/common/intern.h"

#include <stddef.h>
#include <string.h>

#include "syl/common/arena.h"
#include "syl/common/hash.h"
#include "syl/common/map.h"

/**
 * Forward linked list of interned strings
 */
typedef struct intern_t {
  size_t length;         // Size of the interned string
  struct intern_t *next; // Link to next interned string
  char str[];            // String data
} intern_t;

// Memory arena for all interned strings
static arena_t intern_arena;
// Maps string hash to intern list
static map_t intern_map;

const char *intern_string(const char *str) {
  return intern_string_range(str, str + strlen(str));
}

const char *intern_string_range(const char *start, const char *end) {
  size_t length = end - start;
  hash_t hash = hash_bytes(start, length);
  // Map does not allow 0 as key
  uint64_t key = hash > 0 ? hash : 1;

  // Attempt to retrieve intern from map
  intern_t *intern = map_get_from_uint64(&intern_map, key);
  if (intern != NULL) {
    // Interned string may already exist in map
    for (intern_t *iter = intern; iter != NULL; iter = iter->next) {
      if (iter->length == length && strncmp(iter->str, start, length) == 0) {
        // String alread exists in list
        return iter->str;
      }
    }
  }
  // Intern for hash is NULL or does not contain the requested string
  // Create new intern for requested string including null terminator
  intern_t *new_intern =
      arena_alloc(&intern_arena, offsetof(intern_t, str) + length + 1);
  new_intern->length = length;
  // Link to previously retrieved intern or NULL if it did not exist
  new_intern->next = intern;

  // Store string with null terminator
  memcpy(new_intern->str, start, length);
  new_intern->str[length] = 0;

  // Insert or update map entry
  map_put_from_uint64(&intern_map, key, new_intern);
  return new_intern->str;
}