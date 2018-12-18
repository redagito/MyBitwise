#include "syl/lex/keywords.h"

void keywords_init()
{
#define KEYWORD(name) name##_keyword = str_intern(#name); buf_push(keywords, name##_keyword)



#undef KEYWORD
}