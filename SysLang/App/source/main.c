#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef _WIN32
#define DEFAULT_TARGET_OS OS_WIN32
#define DEFAULT_TARGET_ARCH ARCH_X86
#elif defined LINUX
#define DEFAULT_TARGET_OS OS_LINUX
#define DEFAULT_TARGET_ARCH ARCH_X86
#elif
#error "Default target not implemented for this system"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <syl/common/intern.h>
#include <syl/common/log.h>
#include <syl/flag.h>
#include <syl/lex/keywords.h>
#include <syl/path.h>
#include <syl/resolve/package.h>
#include <syl/target.h>
#include <syl/type.h>

static void package_search_path_init(const char *home_path,
                                     const char *search_paths, bool verbose) {
  if (home_path == NULL) {
    // Use environment as default
    home_path = getenv("SYL_HOME");
    if (home_path == NULL) {
      log_fatal("Missing setting for the home directory (where system_packages "
                "is located). "
                "Either set the environment variable SYL_HOME or provide the "
                "path as a command line argument");
    }
  }

  if (verbose) {
    log_info("Home path: %s", home_path);
  }

  // Add provided home path
  char path[MAX_PATH];
  path_copy(path, home_path);
  path_join(path, "system_packages");
  package_add_search_path(path, verbose);

  // Add current dir
  package_add_search_path(".", verbose);

  // Additional search paths as list, separated by ';'
  if (search_paths == NULL) {
    search_paths = getenv("SYL_PATH");
    if (search_paths == NULL) {
      // No additional search paths set
      return;
    }
  }
  const char *start = search_paths;
  for (const char *ptr = search_paths; *ptr != 0; ++ptr) {
    if (*ptr == ';') {
      package_add_search_path_range(start, ptr, verbose);
      start = ptr + 1;
    }
  }

  if (*start != 0) {
    package_add_search_path(start, verbose);
  }
}

static void compiler_init(os_t os, arch_t arch, const char *home_path,
                          const char *search_paths, bool verbose) {
  // Metrics and builtin types
  type_init(os, arch);
  // Language internal keywords
  keywords_init();
  // Path to system packages
  package_search_path_init(home_path, search_paths, verbose);
}

static void print_usage(const char *program_name) {
  printf("Usage: %s [flags] <main-package>\n", program_name);
  flag_print_usage();
}

/**
 * Read and parse environment variables
 */
static void parse_env_vars(int *target_os, int *target_arch) {
  const char *env_target_os = getenv("SYL_OS");
  if (env_target_os != NULL) {
    *target_os = (int)os_from_string(env_target_os);
  }

  const char *env_target_arch = getenv("SYL_ARCH");
  if (env_target_arch != NULL) {
    *target_arch = (int)arch_from_string(env_target_arch);
  }
}

int main(int argc, char **argv) {
  // Settings
  const char *output_name = NULL;
  int target_os = DEFAULT_TARGET_OS;
  int target_arch = DEFAULT_TARGET_ARCH;

  // Home directory, location of system packages
  const char *home_path = NULL;
  // Comma separated list of additional search paths for packages
  const char *search_paths = NULL;

  // Flags
  bool flag_check = false;
  bool flag_lazy = false;
  bool flag_notypeinfo = false;
  bool flag_fullgen = false;
  bool flag_verbose = false;
  bool flag_help = false;

  // Environment variables
  // Parse variables which require mapping
  parse_env_vars(&target_os, &target_arch);

  // File name of the executable
  const char *program_name = path_file(argv[0]);
  ++argv;
  --argc;

  // Command line flags
  flag_add_str("o", &output_name, "file",
               "Output file (default: out_<main-package>.c)");
  flag_add_enum("os", &target_os, "Target operating system", target_os_names(),
                OS_COUNT);
  flag_add_enum("arch", &target_arch, "Target machine architecture",
                target_arch_names(), ARCH_COUNT);
  flag_add_bool("check", &flag_check,
                "Semantic checking without code generation");
  flag_add_bool("lazy", &flag_lazy,
                "Only compile what is reachable from the main package");
  flag_add_bool("notypeinfo", &flag_notypeinfo,
                "Do not generate typeinfo tables");
  flag_add_bool("fullgen", &flag_fullgen,
                "Force full code generation even for unreachable symbols");
  flag_add_str("home", &home_path, "path",
               "Overwrites path to home directory (SYL_HOME)");
  flag_add_str("path", &search_paths, "path",
               "Overwrites additional search paths for packages (SYL_PATH)");
  flag_add_bool("verbose", &flag_verbose, "Extra diagnostic information");
  flag_add_bool("help", &flag_help, "Show usage");

  // Parse without program name
  flag_parse(&argc, &argv);

  // Print help
  if (flag_help) {
    print_usage(program_name);
    return 0;
  }

  // Should only have package name remaining
  if (argc != 1) {
    print_usage(program_name);
    return 1;
  }
  const char *package_name = argv[0];

  // Proper types
  os_t os = target_os;
  arch_t arch = target_arch;

  if (flag_verbose) {
    log_info("Target operating system: %s", os_to_string(os));
    log_info("Target architecture: %s", arch_to_string(arch));
    log_info("Main package: %s", package_name);
  }
  compiler_init(os, arch, home_path, search_paths, flag_verbose);

  package_import("builtin", os, arch, flag_verbose);

  return 0;
}