#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * String utilities
 */

/**
 * Returns true if string does not contain any upper case characters
 */
bool string_islower(const char *str);

/**
 * Searches array of strings and returns index of string position or default
 * if not found
 */
int string_search_array(const char *str, const char **str_array,
                        size_t array_size, int default_value);