#include "testy.h"

#include <syl/common/string_util.h>

TESTY_TEST(string_util) {
  TESTY_CHECK(string_islower("") == true);
  TESTY_CHECK(string_islower("a") == true);
  TESTY_CHECK(string_islower("A") == false);
  TESTY_CHECK(string_islower("123456789") == true);
  TESTY_CHECK(string_islower("-_.:,;#'+*!$%&/()=?") == true);
  TESTY_CHECK(string_islower("abcdefghi") == true);
  TESTY_CHECK(string_islower("ABCDEFGHI") == false);
  TESTY_CHECK(string_islower("abcdefghI") == false);
  TESTY_CHECK(string_islower("ABCDEFGHi") == false);

  const char *str_array[4] = {"foo", "bar", "foobar", "test"};
  TESTY_CHECK(string_search_array("foobar", str_array, 4, -1) == 2);
  TESTY_CHECK(string_search_array("foo", str_array, 4, -1) == 0);
  TESTY_CHECK(string_search_array("test", str_array, 4, -1) == 3);
  TESTY_CHECK(string_search_array("testa", str_array, 4, -1) == -1);
  TESTY_CHECK(string_search_array("", str_array, 4, 10) == 10);
}