#pragma once

#include <stdbool.h>

/**
* Command line flag parsing
*/

/**
* Add flag for boolean values
*/
void flag_add_bool(const char* name, bool* ptr, const char* help);

/**
* Add flag for string values
*/
void flag_add_str(const char* name, const char** ptr, const char* arg_name, const char* help);

/**
* Add flag for enum values
*/
void flag_add_enum(const char* name, int* ptr, const char* help, const char** options, int num_options);

/**
* Print the usage of the current flags
*/
void flag_print_usage();

/**
* Parse the flags from the command line
*/
const char* flag_parse(int* argc_ptr, const char*** argv_ptr);