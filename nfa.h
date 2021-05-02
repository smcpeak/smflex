/* nfa.h */
/* Declarations for nfa.c. */
/* See COPYING for license terms. */

#ifndef NFA_H
#define NFA_H

/* See definitions for documentation. */
void add_accept(int mach, int accepting_number);
int copysingl(int singl, int num);
void dumpnfa(int state1);
int dupmachine(int mach);
void finish_rule(int mach, int variable_trail_rule,
                 int headcnt, int trailcnt);
int link_machines(int first, int last);
void mark_beginning_as_normal(int mach);
int mkbranch(int first, int second);
int mkclos(int state);
int mkopt(int mach);
int mkor(int first, int second);
int mkposcl(int state);
int mkrep(int mach, int lb, int ub);
int mkstate(int sym);
void mkxtion(int statefrom, int stateto);
void new_rule(void);

#endif /* NFA_H */
