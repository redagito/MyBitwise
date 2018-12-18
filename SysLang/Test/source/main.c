#include "testy.h"

// Lex
TESTY_DCL(keywords);

// Common
TESTY_DCL(hash);
TESTY_DCL(buffer);
TESTY_DCL(memory);
TESTY_DCL(arena);
TESTY_DCL(map);
TESTY_DCL(string_util);
TESTY_DCL(intern);

TESTY_DCL(path);
TESTY_DCL(flag);

int main(int argc, char** argv)
{
	// Keep at beginning
	TESTY_INIT(argc, argv);

	// Lex
	// keep at beginning of tests
	TESTY_CALL(keywords);

	// Common
	TESTY_CALL(buffer);
	TESTY_CALL(hash);
	TESTY_CALL(memory);
	TESTY_CALL(arena);
	TESTY_CALL(map);
	TESTY_CALL(string_util);
	TESTY_CALL(intern);

	TESTY_CALL(path);
	TESTY_CALL(flag);


	// Keep at end
	TESTY_END();
}