/* sym.h */
/* Declarations for sym.c. */
/* See COPYING for license terms. */

#ifndef SYM_H
#define SYM_H

#include "flexchar.h"                  /* Char */


struct hash_entry {
  struct hash_entry *prev, *next;
  char *name;
  char *str_val;
  int int_val;
};

typedef struct hash_entry **hash_table;


/* Variables for symbol tables:
 * sctbl - start-condition symbol table
 * ndtbl - name-definition symbol table
 * ccltab - character class text symbol table
 */

#define NAME_TABLE_HASH_SIZE 101
#define START_COND_HASH_SIZE 101
#define CCL_HASH_SIZE 101

extern struct hash_entry *ndtbl[NAME_TABLE_HASH_SIZE];
extern struct hash_entry *sctbl[START_COND_HASH_SIZE];
extern struct hash_entry *ccltab[CCL_HASH_SIZE];


/* See definitions for documentation. */
int addsym(char sym[], char *str_def, int int_def,
           hash_table table, int table_size);
void cclinstal(Char ccltxt[], int cclnum);
int ccllookup(Char ccltxt[]);
struct hash_entry *findsym(char sym[], hash_table table, int table_size);
int hashfunct(char str[], int hash_size);
void ndinstal(char name[], Char definition[]);
Char *ndlookup(char nd[]);
void scextend(void);
void scinstal(char str[], int xcluflg);
int sclookup(char str[]);

#endif /* SYM_H */
