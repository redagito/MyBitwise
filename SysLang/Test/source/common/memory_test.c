#include "testy.h"

#include <stdlib.h>

#include <syl/common/memory.h>

TESTY_TEST(memory)
{
	int* ptr = NULL;
	ptr = xcalloc(10, sizeof(int));
	TESTY_ASSERT(ptr != NULL);
	
	for (int i = 0; i < 10; ++i)
	{
		TESTY_ASSERT(ptr[i] == 0);
	}
	
	ptr = xrealloc(ptr, 12 * sizeof(int));
	TESTY_ASSERT(ptr != NULL);
	free(ptr);
	ptr = NULL;

	ptr = xmalloc(10);
	TESTY_ASSERT(ptr != NULL);
	free(ptr);

	const char text[] = "123456789";
	char* str = xcalloc(10, sizeof(char));
	TESTY_ASSERT(ptr != NULL);

	strcpy(str, text);
	char* str_copy = memdup(str, 10);
	TESTY_ASSERT(str_copy != NULL);
	TESTY_ASSERT(memcmp(str, str_copy, 10) == 0);

	free(str);
	free(str_copy);
}