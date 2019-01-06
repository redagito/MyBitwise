#include "testy.h"

#include <syl/common/hash.h>

TESTY_TEST(hash) {
  // Hash NULL values
  TESTY_CHECK(hash_uint64(0) == 0);
  TESTY_CHECK(hash_ptr(NULL) == 0);
  TESTY_CHECK(hash_mix(hash_uint64(0), hash_uint64(0)) == 0);
  TESTY_CHECK(hash_bytes(NULL, 0) == 14695981039346656037);

  TESTY_CHECK(hash_uint64(1) == 18397679291040277274);
  TESTY_CHECK(hash_uint64(123456789) == 14177905599502729175);
  TESTY_CHECK(hash_uint64(123456789123456789) == 8064043627363279653);
  TESTY_CHECK(hash_bytes("123456789", 9) == 4590379448504041522);
}