/* prefix-main.cc */
/* Program that runs several lexers. */

#include "out/prefix1-c.yy.h"          /* prefix1_lexer_t */
#include "out/prefix2-c.yy.h"          /* prefix2_lexer_t */
#include "out/prefix3-cpp.yy.h"        /* prefix3FlexLexer */
#include "out/prefix4-cpp.yy.h"        /* prefix4FlexLexer */


char const *input_text =
  "Line one of input_text.\n"
  "The password is: prefix\n";


void run_prefix1()
{
  prefix1_lexer_t lexer;
  prefix1_construct(&lexer);
  prefix1_scan_string(&lexer, input_text);
  while (prefix1_lex(&lexer))
    {}
  prefix1_destroy(&lexer);
  prefix1_check_for_memory_leaks();
}

void run_prefix2()
{
  prefix2_lexer_t lexer;
  prefix2_construct(&lexer);
  prefix2_scan_string(&lexer, input_text);
  while (prefix2_lex(&lexer))
    {}
  prefix2_destroy(&lexer);
  prefix2_check_for_memory_leaks();
}

void run_prefix3()
{
  {
    prefix3FlexLexer lexer;
    lexer.yym_scan_string(input_text);
    while (lexer.yym_lex())
      {}
  }
  prefix3FlexLexer::yym_check_for_memory_leaks();
}

void run_prefix4()
{
  {
    prefix4FlexLexer lexer;
    lexer.yym_scan_string(input_text);
    while (lexer.yym_lex())
      {}
  }
  prefix4FlexLexer::yym_check_for_memory_leaks();
}

int main()
{
  run_prefix1();
  run_prefix2();
  run_prefix3();
  run_prefix4();
  return 0;
}

/* EOF */
