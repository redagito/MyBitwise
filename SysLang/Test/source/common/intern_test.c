#include "testy.h"

#include <syl/common/intern.h>

TESTY_TEST(intern)
{
	char str1[] = "Hello world!";
	const char* str1_interned = intern_string(str1);
	TESTY_ASSERT(str1_interned != NULL);
	TESTY_ASSERT(strcmp(str1, str1_interned) == 0);
	TESTY_ASSERT(str1_interned == intern_string(str1));

	// Same as str1
	char str2[] = "Hello world!";
	TESTY_ASSERT(str2 != str1);

	const char* str2_interned = intern_string(str2);
	TESTY_ASSERT(str2_interned != NULL);
	TESTY_ASSERT(strcmp(str2, str2_interned) == 0);
	TESTY_ASSERT(str2 != str2_interned);
	TESTY_ASSERT(str1_interned == str2_interned);
	TESTY_ASSERT(intern_string(str1) == intern_string(str2));

	// Different, uppercase 'W'
	char str3[] = "Hello World!";
	TESTY_ASSERT(strcmp(str1, str3) != 0);

	const char* str3_interned = intern_string(str3);
	TESTY_ASSERT(str3_interned != NULL);
	TESTY_ASSERT(strcmp(str3, str3_interned) == 0);
	TESTY_ASSERT(str3 != str3_interned);
	TESTY_ASSERT(str1_interned != str3_interned);
	TESTY_ASSERT(intern_string(str1) != intern_string(str3));
	TESTY_ASSERT(strcmp(str1_interned, str3_interned) != 0);
}