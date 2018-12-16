#include "syl/common/log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static void print_error_message(const char* type, const char* fmt, va_list args)
{
	fprintf(stderr, "%s: ", type);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
}

void error(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	print_error_message("ERROR", fmt, args);
	va_end(args);
}

void fatal(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	print_error_message("FATAL", fmt, args);
	va_end(args);
	exit(1);
}
