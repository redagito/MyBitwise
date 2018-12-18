#pragma once

/**
* Language keywords
*/

const char* typedef_keyword;
const char* enum_keyword;
const char* struct_keyword;
const char* union_keyword;
const char* var_keyword;
const char* const_keyword;
const char* func_keyword;
const char* sizeof_keyword;
const char* alignof_keyword;
const char* typeof_keyword;
const char* offsetof_keyword;
const char* break_keyword;
const char* continue_keyword;
const char* return_keyword;
const char* if_keyword;
const char* else_keyword;
const char* while_keyword;
const char* do_keyword;
const char* for_keyword;
const char* switch_keyword;
const char* case_keyword;
const char* default_keyword;
const char* import_keyword;
const char* goto_keyword;

const char* first_keyword;
const char* last_keyword;
const char* *keywords;

const char* always_name;
const char* foreign_name;
const char* complete_name;
const char* assert_name;
const char* declare_note_name;
const char* static_assert_name;

void keywords_init();