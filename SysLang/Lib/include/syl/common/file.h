#pragma once

#include <stdbool.h>
#include <stdint.h>

/**
 * File handling
 */

/**
 * Read file into memory and return content as string
 */
char *file_read(const char *path);

/**
 * Write string to file.
 * Overwrites existing contents.
 */
bool file_write(const char *path, const char *buffer, size_t length);