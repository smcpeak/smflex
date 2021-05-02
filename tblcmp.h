/* tblcmp.h */
/* Declarations for tblcmp.c. */
/* See COPYING for license terms. */

#ifndef TBLCMP_H
#define TBLCMP_H

/* See definitions for documentation. */
void bldtbl(int state[], int statenum, int totaltrans,
            int comstate, int comfreq);
void cmptmps(void);
void expand_nxt_chk(void);
int find_table_space(int *state, int numtrans);
void inittbl(void);
void mkdeftbl(void);
void mkentry(int *state, int numchars, int statenum,
             int deflink, int totaltrans);
void mk1tbl(int state, int sym, int onenxt, int onedef);
void mkprot(int state[], int statenum, int comstate);
void mktemplate(int state[], int statenum, int comstate);
void mv2front(int qelm);
void place_state(int *state, int statenum, int transnum);
void stack1(int statenum, int sym, int nextstate, int deflink);
int tbldiff(int state[], int pr, int ext[]);

#endif /* TBLCMP_H */
