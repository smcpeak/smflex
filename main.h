/* main.h */
/* Declarations for main.c. */
/* See COPYING for license terms. */

#ifndef MAIN_H
#define MAIN_H

/* See definitions for documentation. */
void check_options(void);
void flexend(int exit_status);
void flexinit(int argc, char **argv);
void readin(void);
void set_up_initial_allocations(void);
void usage(void);

#endif /* MAIN_H */
