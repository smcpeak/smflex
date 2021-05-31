/* test-growstring.c */
/* Unit tests for 'growstring' module. */
/* See COPYING for license terms. */

#include "growstring.h"                /* module under test */

#include <assert.h>                    /* assert */
#include <stdio.h>                     /* printf */


static void print_gs(growstring_t *gs)
{
  if (gs->length == 0) {
    printf("empty\n");
    assert(gs->array == NULL);
    assert(gs->allocated == 0);
  }
  else {
    size_t i;

    printf("length: %d\n", (int)gs->length);
    printf("allocated: %d\n", (int)gs->allocated);
    printf("array: \"");
    for (i=0; i < gs->length; i++) {
      int c = (unsigned char)(gs->array[i]);
      if (c == '"') {
        printf("\\\"");
      }
      else if (32 <= c && c <= 126 ) {
        printf("%c", c);
      }
      else {
        printf("\\x%02X", c);
      }
    }
    printf("\"\n");

    assert(gs->array[gs->length] == 0);
  }
}


static void test1()
{
  growstring_t gs = GROWSTRING_EMPTY_INIT;

  printf("---- test1 ----\n");
  print_gs(&gs);

  growstring_init(&gs);
  print_gs(&gs);

  growstring_append_c_str(&gs, "Hello");
  print_gs(&gs);

  growstring_append_c_str(&gs, ", world!");
  print_gs(&gs);
}


static void test2()
{
  growstring_t gs = GROWSTRING_EMPTY_INIT;

  printf("---- test2 ----\n");
  print_gs(&gs);

  growstring_append_c_str(&gs, "there is a NUL here -->");
  growstring_append(&gs, "\0", 1);
  growstring_append_c_str(&gs, "<--");
  print_gs(&gs);
}


static void test3()
{
  growstring_t gs = GROWSTRING_EMPTY_INIT;
  int i;

  printf("---- test3 ----\n");
  for (i=0; i < 1000000; i++) {
    growstring_append(&gs, "x", 1);
  }

  printf("len: %d\n", (int)gs.length);
  printf("alloc: %d\n", (int)gs.allocated);
}


int main()
{
  test1();
  test2();
  test3();
  return 0;
}


/* EOF */
