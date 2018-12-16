#include "syl/common/string_util.h"

#include <ctype.h>

bool string_islower(const char* str)
{
	while (*str != 0)
	{
		if (isalpha(*str) && !islower(*str))
		{
			return false;
		}
		++str;
	}
	return true;
}