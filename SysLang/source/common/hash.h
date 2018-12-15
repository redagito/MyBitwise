#pragma once

#include <stdint.h>

/**
* Hashing
*/

typedef uint64_t hash_t;

hash_t hash_uint64(uint64_t value);

hash_t hash_ptr(const void* ptr);

hash_t hash_mix(hash_t a, hash_t b);

hash_t hash_bytes(const void* ptr, size_t size);