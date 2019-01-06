#pragma once

#include <stdint.h>

/**
 * Hashing
 */

typedef uint64_t hash_t;

/**
 * Caclculate hash from uint64
 */
hash_t hash_uint64(uint64_t value);

/**
 * Calculate hash from address
 */
hash_t hash_ptr(const void *ptr);

/**
 * Mix 2 hash values
 */
hash_t hash_mix(hash_t a, hash_t b);

/**
 * Calculate hash for memory area
 */
hash_t hash_bytes(const void *ptr, size_t size);