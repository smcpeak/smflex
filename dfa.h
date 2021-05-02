/* dfa.h */
/* Declarations for dfa.c. */
/* See COPYING for license terms. */

#ifndef DFA_H
#define DFA_H

#include <stdio.h>                     /* FILE */

/* See definitions for documentation. */
void check_for_backing_up(int ds, int state[]);
void check_trailing_context(int *nfa_states, int num_states,
                            int *accset, int nacc);
void dump_associated_rules(FILE *file, int ds);
void dump_transitions(FILE *file, int state[]);
void increase_max_dfas(void);
void ntod(void);
int snstods(int sns[], int numstates, int accset[], int nacc,
            int hashval, int *newds_addr);
int symfollowset(int ds[], int dsize, int transsym, int nset[]);
void sympartition(int ds[], int numstates, int symlist[], int duplist[]);

#endif /* DFA_H */
