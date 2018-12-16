#include "testy.h"

TESTY_DCL(hash);
TESTY_DCL(buffer);

int main(int argc, char** argv)
{
	TESTY_CALL(buffer);
	TESTY_CALL(hash);

	return 0;
}