#include "testy.h"

TESTY_DCL(hash);
TESTY_DCL(buffer);
TESTY_DCL(memory);
TESTY_DCL(arena);
TESTY_DCL(map);
TESTY_DCL(string_util);
TESTY_DCL(string_intern);

int main(int argc, char** argv)
{
	TESTY_CALL(buffer);
	TESTY_CALL(hash);
	TESTY_CALL(memory);
	TESTY_CALL(arena);
	TESTY_CALL(map);
	TESTY_CALL(string_util);
	TESTY_CALL(string_intern);

	return 0;
}