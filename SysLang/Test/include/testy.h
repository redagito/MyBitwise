#pragma once

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/**
* Unit test library
*/

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))

// Console color codes
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct testy_stats_t
{
	uint32_t passed_count;
	uint32_t failed_count;
} testy_stats_t;

/**
* Declare tests outside of main
*/
#define TESTY_TEST(X) void X ## _test(testy_stats_t* testy_stats)
#define TESTY_DCL(X) extern TESTY_TEST(X)

/**
* Run tests inside of main
*/
#define TESTY_CALL(X) do {\
printf("Running test: %s...\n", #X);\
testy_stats_t testy_stats_local;\
testy_stats_local.passed_count = 0;\
testy_stats_local.failed_count = 0;\
X ## _test(&testy_stats_local);\
printf("Passed: %u\nFailed: %u\n", testy_stats_local.passed_count, testy_stats_local.failed_count);\
if (testy_stats_local.passed_count + testy_stats_local.failed_count == 0)\
{ printf(ANSI_COLOR_YELLOW "Test EMPTY\n\n" ANSI_COLOR_RESET); }\
else if(testy_stats_local.failed_count == 0)\
{ printf(ANSI_COLOR_GREEN "Test PASSED\n\n" ANSI_COLOR_RESET); }\
else { printf(ANSI_COLOR_RED "Test FAILED\n\n" ANSI_COLOR_RESET); }\
} while(false)

#define TESTY_ON_FAIL(X) do{\
testy_stats->failed_count++; \
printf(ANSI_COLOR_RED "Check failed: %s at %s:%u\n" ANSI_COLOR_RESET, #X, __FILENAME__, __LINE__); \
}while(false)

// Checks expression and updates stats
#define TESTY_CHECK(X) do{\
if((X)) { testy_stats->passed_count++; }\
else {\
TESTY_ON_FAIL(X);\
} } while (false)

// As above but returns on fail
#define TESTY_ASSERT(X) do\
{\
if((X)) { testy_stats->passed_count++; }\
else {\
TESTY_ON_FAIL(X);\
return; \
} } while (false)