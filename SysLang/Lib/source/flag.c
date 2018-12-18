#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "syl/flag.h"

#include <stdio.h>
#include <string.h>

#include "syl/common/buffer.h"
#include "syl/path.h"

/**
* Flag data type
*/
typedef enum
{
	FLAG_BOOL,
	FLAG_STR,
	FLAG_ENUM
} flag_type_t;

typedef struct flag_t
{
	flag_type_t type; // Flag value type
	const char* name;
	const char* help; // Hep text
	const char** options;
	const char* arg_name;
	size_t num_options;

	// Default value for each flag type
	// TODO Why not union?
	struct
	{
		int* i;
		bool* b;
		const char** s;
	} ptr;
} flag_t;

// Stretchy buffer of flags
static flag_t* flags;

static flag_t* flag_get(const char* name)
{
	// Linear search for flag by name
	for (size_t i = 0; i < buffer_length(flags); ++i)
	{
		if (strcmp(flags[i].name, name) == 0)
		{
			return &flags[i];
		}
	}
	return NULL;
}

void flag_add_bool(const char* name, bool* ptr, const char* help)
{
	buffer_push(flags, (flag_t) { .type = FLAG_BOOL, .name = name, .help = help, .ptr.b = ptr });
}

void flag_add_str(const char* name, const char** ptr, const char* arg_name, const char* help)
{
	buffer_push(flags, (flag_t) { .type = FLAG_STR, .name = name, .help = help, .arg_name = arg_name, .ptr.s = ptr });
}

void flag_add_enum(const char* name, int* ptr, const char* help, const char** options, int num_options)
{
	buffer_push(flags, (flag_t) { .type = FLAG_ENUM, .name = name, .help = help, .ptr.i = ptr, .options = options, .num_options = num_options });
}

static void flag_print_usage_enum(flag_t flag, char(*format)[256], char(*note)[256])
{
	char* end = *format + sizeof(*format);
	char* ptr = *format;
	// Print and move ptr
	ptr += snprintf(ptr, end - ptr, "%s <", flag.name);

	// Print options
	for (size_t i = 0; i < flag.num_options; ++i)
	{
		ptr += snprintf(ptr, end - ptr, "%s%s", i == 0 ? "" : "|", flag.options[i]);
		// Prints note for default value
		// TODO Why in loop and not just afterwards?
		if (i == *flag.ptr.i)
		{
			snprintf(*note, sizeof(*note), " (default: %s)", flag.options[i]);
		}
	}
	snprintf(ptr, end - ptr, ">");
}

static void flag_print_usage_str(flag_t flag, char(*format)[256], char(*note)[256])
{
	snprintf(*format, sizeof(*format), "%s <%s>", flag.name, flag.arg_name ? flag.arg_name : "value");
	if (*flag.ptr.s != NULL)
	{
		snprintf(*note, sizeof(*note), "(default: %s)", *flag.ptr.s);
	}
}

static void flag_print_usage_default(flag_t flag, char (*format)[256], char (*note)[256])
{
	snprintf(*format, sizeof(*format), "%s", flag.name);
}

void flag_print_usage()
{
	printf("Flags:\n");
	for (size_t i = 0; i < buffer_length(flags); ++i)
	{
		flag_t flag = flags[i];
		char note[256] = {0};
		char format[256];

		switch (flag.type)
		{
		case FLAG_STR:
			flag_print_usage_str(flag, &format, &note);
			break;
		case FLAG_ENUM:
			flag_print_usage_enum(flag, &format, &note);
			break;
		case FLAG_BOOL:
		default:
			flag_print_usage_default(flag, &format, &note);
			break;
		}
		printf(" -%-32s %s%s\n", format, flag.help ? flag.help : "", note);
	}
}

const char* flag_parse(int* argc_ptr, const char*** argv_ptr)
{
	return NULL;
}