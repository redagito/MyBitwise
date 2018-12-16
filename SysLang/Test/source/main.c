#include "testy.h"

TESTY_DCL(hash);
TESTY_DCL(buffer);
TESTY_DCL(memory);
TESTY_DCL(arena);

int main(int argc, char** argv)
{
	TESTY_CALL(buffer);
	TESTY_CALL(hash);
	TESTY_CALL(memory);
	TESTY_CALL(arena);

	return 0;
}