#include "syl/common/hash.h"

hash_t hash_uint64(uint64_t value)
{
	hash_t hash = 0xff51afd7ed558ccd * value;
	hash ^= hash >> 32;
	return hash;
}

hash_t hash_ptr(const void* ptr)
{
	return hash_uint64((uintptr_t)ptr);
}

hash_t hash_mix(hash_t a, hash_t b)
{
	return hash_uint64(a ^ b);
}

hash_t hash_bytes(const void* ptr, size_t size)
{
	// Init hash
	hash_t hash = 0xcbf29ce484222325;
	const char* buffer = (const char*)ptr;

	// Hash bytes 
	for (size_t i = 0; i < size; ++i)
	{
		hash ^= buffer[i];
		hash *= 0x100000001b3;
		hash ^= hash >> 32;
	}
	return hash;
}