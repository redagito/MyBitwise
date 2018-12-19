#include "syl/common/log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// Console color codes
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

static void log_message_internal(FILE* file, const char* type, const char* fmt, va_list args)
{
	fprintf(stderr, "[%s]: ", type);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
}

void log_warning(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	log_warning_va(fmt, args);
	va_end(args);
}

void log_warning_va(const char* fmt, va_list args)
{
	log_message_internal(stdout, ANSI_COLOR_YELLOW "WARNING" ANSI_COLOR_RESET, fmt, args);
}

void log_error(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	log_error_va(fmt, args);
	va_end(args);
}

void log_error_va(const char* fmt, va_list args)
{
	log_message_internal(stderr, ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET, fmt, args);
}

void log_fatal(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	log_fatal_va(fmt, args);
	va_end(args);
	exit(1);
}

void log_fatal_va(const char* fmt, va_list args)
{
	log_message_internal(stderr, ANSI_COLOR_RED "FATAL" ANSI_COLOR_RESET, fmt, args);
}