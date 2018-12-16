#include "testy.h"

TESTY_DCL(hash);
TESTY_DCL(buffer);
TESTY_DCL(memory);

int main(int argc, char** argv)
{
	TESTY_CALL(buffer);
	TESTY_CALL(hash);
	TESTY_CALL(memory);

	return 0;
}