#include <stdio.h>

#include <syl/path.h>
#include <syl/flag.h>

void print_usage(const char* program_name)
{
	printf("Usage: %s [flags] <main-package>\n", program_name);
	flag_print_usage();
}

int main(int argc, char** argv)
{
	// Program settings
	const char* output_name = NULL;
	int target_os = 0;
	int target_arch = 0;
	bool flag_check = false;
	bool flag_lazy = false;
	bool flag_notypeinfo = false;
	bool flag_fullgen = false;
	bool flag_verbose = false;
	bool flag_help = false;

	// File name of the executable
	const char* program_name = path_file(argv[0]);
	++argv;
	--argc;

	// Set program flags
	flag_add_str("o", &output_name, "file", "Output file (default: out_<main-package>.c)");
	flag_add_enum("os", &target_os, "Target operating system", NULL, 0);
	flag_add_enum("arch", &target_arch, "Target machine architecture", NULL, 0);
	flag_add_bool("check", &flag_check, "Semantic checking without code generation");
	flag_add_bool("lazy", &flag_lazy, "Only compile what is reachable from the main package");
	flag_add_bool("notypeinfo", &flag_notypeinfo, "Do not generate typeinfo tables");
	flag_add_bool("fullgen", &flag_fullgen, "Force full code generation even for unreachable symbols");
	flag_add_bool("verbose", &flag_verbose, "Extra diagnostic information");
	flag_add_bool("help", &flag_help, "Show usage");
	
	// Parse flags
	flag_parse(&argc, &argv);

	// Print help
	if (flag_help)
	{
		print_usage(program_name);
		return 0;
	}

	// Should only have package name remaining
	if (argc != 1)
	{
		print_usage(program_name);
		return 1;
	}
	return 0;
}