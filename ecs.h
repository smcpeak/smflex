/* ecs.h */
/* Declarations for ecs.c. */
/* See COPYING for license terms. */

#ifndef ECS_H
#define ECS_H

#include "flexchar.h"                  /* Char */

/* See definitions for documentation. */
void ccl2ecl(void);
int cre8ecs(int fwd[], int bck[], int num);
void mkeccl(Char ccls[], int lenccl, int fwd[], int bck[],
            int llsiz, int NUL_mapping);
void mkechar(int tch, int fwd[], int bck[]);

#endif /* ECS_H */
