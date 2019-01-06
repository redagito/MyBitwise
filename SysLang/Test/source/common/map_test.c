#include "testy.h"

#include <syl/common/map.h>

TESTY_TEST(map) {
  map_t map = map_create();
  const size_t n = 1024;

  // Fill with values
  for (size_t i = 1; i <= n; ++i) {
    map_put(&map, (void *)i, (void *)(i + 1));
  }
  TESTY_ASSERT(map.size == 1024);

  // Retrieve and check
  for (size_t i = 1; i <= n; ++i) {
    void *value = map_get(&map, (void *)i);
    TESTY_ASSERT(value == (void *)(i + 1));
  }
}