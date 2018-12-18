#include "syl/lex/keywords.h"

#include <assert.h>

#include "syl/common/log.h"
#include "syl/common/intern.h"
#include "syl/common/arena.h"
#include "syl/common/buffer.h"

// Stretchy buffer containing all keywords
static const char** keywords;

// Pointer to first and last keywords
// All interned keywords must be between these memory locations
static const char* first_keyword;
static const char* last_keyword;

// Initialization flag
static bool initialized;

void keywords_init()
{
	if (initialized)
	{
		log_warning("Failed to initialize keywords, already initialized");
		return;
	}

#define KEYWORD(name) name##_keyword = intern_string(#name); buffer_push(keywords, name##_keyword)
	// Keywords must be stored on the same block of an arena
	// May cause grow
	KEYWORD(typedef);
	// Ensure everything is on the same block
	arena_disable_grow();
	KEYWORD(enum);
	KEYWORD(struct);
	KEYWORD(union);
	KEYWORD(const);
	KEYWORD(var);
	KEYWORD(func);
	KEYWORD(import);
	KEYWORD(goto);
	KEYWORD(sizeof);
	KEYWORD(alignof);
	KEYWORD(typeof);
	KEYWORD(offsetof);
	KEYWORD(break);
	KEYWORD(continue);
	KEYWORD(return);
	KEYWORD(if);
	KEYWORD(else);
	KEYWORD(while);
	KEYWORD(do);
	KEYWORD(for);
	KEYWORD(switch);
	KEYWORD(case);
	KEYWORD(default);
	arena_enable_grow();
#undef KEYWORD

	first_keyword = typedef_keyword;
	last_keyword = default_keyword;

	// Additional internal/reserved identifiers
#define RESERVED(name) name##_name = intern_string(#name)
	RESERVED(always);
	RESERVED(foreign);
	RESERVED(complete);
	RESERVED(assert);
	RESERVED(declare_note);
	RESERVED(static_assert);
#undef RESERVED

	// Init finished
	initialized = true;
}

bool keywords_is_keyword(const char* str)
{
	assert(initialized);
	return first_keyword <= str && str <= last_keyword;
}