#pragma once

#include <stdarg.h>

/**
 * Message logging functionality
 */

/**
 * Print info message
 */
void log_info(const char *fmt, ...);
void log_info_va(const char *fmt, va_list args);

/**
 * Prints error message
 */
void log_warning(const char *fmt, ...);
void log_warning_va(const char *fmt, va_list args);

/**
 * Prints error message
 */
void log_error(const char *fmt, ...);
void log_error_va(const char *fmt, va_list args);

/**
 * Prints fatal error message and exits the program
 */
void log_fatal(const char *fmt, ...);
void log_fatal_va(const char *fmt, va_list args);