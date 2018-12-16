#include "testy.h"

#include <syl/common/string_util.h>

TESTY_TEST(string_util)
{
	TESTY_CHECK(string_islower("") == true);
	TESTY_CHECK(string_islower("a") == true);
	TESTY_CHECK(string_islower("A") == false);
	TESTY_CHECK(string_islower("123456789") == true);
	TESTY_CHECK(string_islower("-_.:,;#'+*!$%&/()=?") == true);
	TESTY_CHECK(string_islower("abcdefghi") == true);
	TESTY_CHECK(string_islower("ABCDEFGHI") == false);
	TESTY_CHECK(string_islower("abcdefghI") == false);
	TESTY_CHECK(string_islower("ABCDEFGHi") == false);
}