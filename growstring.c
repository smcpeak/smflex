/* growstring.c */
/* Code for growstring.h. */
/* See COPYING for license terms. */

#include "growstring.h"                /* this module */

#include <assert.h>                    /* assert */
#include <stdio.h>                     /* fprintf */
#include <stdlib.h>                    /* malloc, abort, etc. */
#include <string.h>                    /* strlen, strcpy, etc. */

#define SIZE_T_MAX ((size_t)(-1))


void growstring_init(growstring_t *gs)
{
  gs->array = NULL;
  gs->length = 0;
  gs->allocated = 0;
}


void growstring_append(growstring_t *gs, char const *src, size_t srclen)
{
  if (srclen == 0) {
    /* Don't allocate if 'gs' is already empty. */
    return;
  }

  if (gs->array == NULL) {
    /* Initial allocation exactly matches 'srclen'. */
    assert(gs->length == 0);
    assert(gs->allocated == 0);
    gs->allocated = srclen + 1;
    gs->array = (char*)malloc(gs->allocated);
    if (!gs->array) {
      fprintf(stderr, "growstring_append: failed to allocate gs->array\n");
      abort();
    }
  }

  else {
    size_t required = gs->length + 1 + srclen;
    if (required <= srclen) {
      fprintf(stderr, "growstring_append: overflow in 'required'\n");
      abort();
    }

    if (gs->allocated < required) {
      size_t new_allocated;
      char *new_array;

      /* Calculate new size with at least exponential growth. */
      assert(gs->allocated > 0);
      if (gs->allocated > SIZE_T_MAX / 2) {
        fprintf(stderr, "growstring_append: 'allocated' too big to expand\n");
        abort();
      }
      new_allocated = gs->allocated * 2;
      if (new_allocated < required) {
        new_allocated = required;
      }

      /* Allocate the new array. */
      new_array = (char*)malloc(new_allocated);
      if (!new_array) {
        fprintf(stderr, "growstring_append: failed to allocate new_array\n");
        abort();
      }

      /* Copy the existing data. */
      memcpy(new_array, gs->array, gs->length);

      /* Replace the old array with the new. */
      free(gs->array);
      gs->array = new_array;
      gs->allocated = new_allocated;
    }
  }

  assert(gs->allocated >= gs->length + 1 + srclen);

  memcpy(gs->array + gs->length, src, srclen);
  gs->length += srclen;
  gs->array[gs->length] = 0;
}


void growstring_append_c_str(growstring_t *gs, char const *src)
{
  growstring_append(gs, src, strlen(src));
}


void growstring_clear(growstring_t *gs)
{
  if (gs->array) {
    free(gs->array);
  }
  gs->array = NULL;
  gs->length = 0;
  gs->allocated = 0;
}


/* EOF */
