#include "testy.h"

#include <syl/flag.h>

TESTY_TEST(flag)
{
	const char* enum_options[] = {"FOO", "BAR", "BAFO", "FOBA", "FOFO"};

	bool b = false;
	int i = 1;
	const char* s = "strdef";
	flag_add_bool("boolean", &b, "A boolean flag");
	flag_add_enum("enum", &i, "An enum flag", enum_options, sizeof(enum_options) / sizeof(*enum_options));
	flag_add_str("string", &s, "strfag", "A string flag");
	flag_print_usage();
}