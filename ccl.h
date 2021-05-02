/* ccl.h */
/* Declarations for ccl.c. */
/* See COPYING for license terms. */

#ifndef CCL_H
#define CCL_H

#include <stdio.h>                     /* FILE */

/* See definitions for documentation. */
void ccladd(int cclp, int ch);
int cclinit(void);
void cclnegate(int cclp);
void list_character_set(FILE *file, int cset[]);

#endif /* CCL_H */
