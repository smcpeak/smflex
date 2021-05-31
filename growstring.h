/* growstring.h */
/* Growable string of characters. */
/* See COPYING for license terms. */

#ifndef GROWSTRING_H
#define GROWSTRING_H

#include <stddef.h>                    /* size_t, NULL */

#ifdef __cplusplus
extern "C" {
#endif

/* This module aborts on any memory allocation failure. */

/* A string (vector) of characters that can grow.  It is safe to
 * initialize all elements to 0 or NULL to mean an empty string. */
typedef struct growstring_struct {
  /* Dynamically-allocated array of 'allocated' characters, or NULL when
   * the string is empty.  When not NULL, the element at index 'length'
   * is 0.  There may be other 0 elements before that. */
  char *array;

  /* Number of characters in the string. */
  size_t length;

  /* Number of characters allocated in 'array', or 0 if the string is
   * empty.  If not zero, this is always at least 'length+1'. */
  size_t allocated;

} growstring_t;

/* As future-proofing in case 'growstring_t' changes, you can initialize
 * a variable of that type with this to make it empty. */
#define GROWSTRING_EMPTY_INIT { NULL, 0, 0 }


/* Initialize 'gs' as an empty string.  It is not necessary to use this
 * (it is fine to directly initialize the fields or use
 * GROWSTRING_EMPTY_INIT), but this is provided for completeness and
 * convenience. */
void growstring_init(growstring_t *gs);

/* Append 'srclen' characters from 'src'. */
void growstring_append(growstring_t *gs, char const *src, size_t srclen);

/* Append characters from 'src' until a 0 byte is found. */
void growstring_append_c_str(growstring_t *gs, char const *src);

/* Empty 'gs', releasing its memory in the process. */
void growstring_clear(growstring_t *gs);

#ifdef __cplusplus
}
#endif

#endif /* GROWSTRING_H */
