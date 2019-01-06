#include "testy.h"

#include <syl/lex/keywords.h>

TESTY_TEST(keywords) {
  TESTY_ASSERT(typedef_keyword == NULL);
  TESTY_ASSERT(enum_keyword == NULL);
  TESTY_ASSERT(struct_keyword == NULL);
  TESTY_ASSERT(union_keyword == NULL);
  TESTY_ASSERT(var_keyword == NULL);
  TESTY_ASSERT(const_keyword == NULL);
  TESTY_ASSERT(func_keyword == NULL);
  TESTY_ASSERT(sizeof_keyword == NULL);
  TESTY_ASSERT(alignof_keyword == NULL);
  TESTY_ASSERT(typeof_keyword == NULL);
  TESTY_ASSERT(offsetof_keyword == NULL);
  TESTY_ASSERT(break_keyword == NULL);
  TESTY_ASSERT(continue_keyword == NULL);
  TESTY_ASSERT(return_keyword == NULL);
  TESTY_ASSERT(if_keyword == NULL);
  TESTY_ASSERT(else_keyword == NULL);
  TESTY_ASSERT(while_keyword == NULL);
  TESTY_ASSERT(do_keyword == NULL);
  TESTY_ASSERT(for_keyword == NULL);
  TESTY_ASSERT(switch_keyword == NULL);
  TESTY_ASSERT(case_keyword == NULL);
  TESTY_ASSERT(default_keyword == NULL);
  TESTY_ASSERT(import_keyword == NULL);
  TESTY_ASSERT(goto_keyword == NULL);

  keywords_init();

  TESTY_ASSERT(typedef_keyword != NULL);
  TESTY_ASSERT(enum_keyword != NULL);
  TESTY_ASSERT(struct_keyword != NULL);
  TESTY_ASSERT(union_keyword != NULL);
  TESTY_ASSERT(var_keyword != NULL);
  TESTY_ASSERT(const_keyword != NULL);
  TESTY_ASSERT(func_keyword != NULL);
  TESTY_ASSERT(sizeof_keyword != NULL);
  TESTY_ASSERT(alignof_keyword != NULL);
  TESTY_ASSERT(typeof_keyword != NULL);
  TESTY_ASSERT(offsetof_keyword != NULL);
  TESTY_ASSERT(break_keyword != NULL);
  TESTY_ASSERT(continue_keyword != NULL);
  TESTY_ASSERT(return_keyword != NULL);
  TESTY_ASSERT(if_keyword != NULL);
  TESTY_ASSERT(else_keyword != NULL);
  TESTY_ASSERT(while_keyword != NULL);
  TESTY_ASSERT(do_keyword != NULL);
  TESTY_ASSERT(for_keyword != NULL);
  TESTY_ASSERT(switch_keyword != NULL);
  TESTY_ASSERT(case_keyword != NULL);
  TESTY_ASSERT(default_keyword != NULL);
  TESTY_ASSERT(import_keyword != NULL);
  TESTY_ASSERT(goto_keyword != NULL);

  TESTY_ASSERT(keywords_is_keyword("bla") == false);
  TESTY_ASSERT(keywords_is_keyword(NULL) == false);

  TESTY_ASSERT(keywords_is_keyword(typedef_keyword));
  TESTY_ASSERT(keywords_is_keyword(enum_keyword));
  TESTY_ASSERT(keywords_is_keyword(struct_keyword));
  TESTY_ASSERT(keywords_is_keyword(union_keyword));
  TESTY_ASSERT(keywords_is_keyword(var_keyword));
  TESTY_ASSERT(keywords_is_keyword(const_keyword));
  TESTY_ASSERT(keywords_is_keyword(func_keyword));
  TESTY_ASSERT(keywords_is_keyword(sizeof_keyword));
  TESTY_ASSERT(keywords_is_keyword(alignof_keyword));
  TESTY_ASSERT(keywords_is_keyword(typeof_keyword));
  TESTY_ASSERT(keywords_is_keyword(offsetof_keyword));
  TESTY_ASSERT(keywords_is_keyword(break_keyword));
  TESTY_ASSERT(keywords_is_keyword(continue_keyword));
  TESTY_ASSERT(keywords_is_keyword(return_keyword));
  TESTY_ASSERT(keywords_is_keyword(if_keyword));
  TESTY_ASSERT(keywords_is_keyword(else_keyword));
  TESTY_ASSERT(keywords_is_keyword(while_keyword));
  TESTY_ASSERT(keywords_is_keyword(do_keyword));
  TESTY_ASSERT(keywords_is_keyword(for_keyword));
  TESTY_ASSERT(keywords_is_keyword(switch_keyword));
  TESTY_ASSERT(keywords_is_keyword(case_keyword));
  TESTY_ASSERT(keywords_is_keyword(default_keyword));
  TESTY_ASSERT(keywords_is_keyword(import_keyword));
  TESTY_ASSERT(keywords_is_keyword(goto_keyword));
}