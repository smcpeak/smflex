/* flexdef - definitions file for smflex */

/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Vern Paxson.
 *
 * The United States Government has rights in this work pursuant
 * to contract no. DE-AC03-76SF00098 between the United States
 * Department of Energy and the University of California.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided
 * that: (1) source distributions retain this entire copyright notice and
 * comment, and (2) distributions including binaries display the following
 * acknowledgement:  ``This product includes software developed by the
 * University of California, Berkeley and its contributors'' in the
 * documentation or other materials provided with the distribution and in
 * all advertising materials mentioning features or use of this software.
 * Neither the name of the University nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef FLEXDEF_H
#define FLEXDEF_H


/* As an aid for the internationalization patch to smflex, which
 * is maintained outside this distribution for copyright reasons.
 */
#define _(String) (String)


/* Maximum line length we'll have to deal with. */
#define MAXLINE 2048


#ifndef MIN
#  define MIN(x,y) ((x) < (y) ? (x) : (y))
#endif
#ifndef MAX
#  define MAX(x,y) ((x) > (y) ? (x) : (y))
#endif
#ifndef ABS
#  define ABS(x) ((x) < 0 ? -(x) : (x))
#endif


#define true 1
#define false 0
#define unspecified -1


/* A note on the following masks.  They are used to mark accepting numbers
 * as being special.  As such, they implicitly limit the number of accepting
 * numbers (i.e., rules) because if there are too many rules the rule numbers
 * will overload the mask bits.  Fortunately, this limit is \large/ (0x2000 ==
 * 8192) so unlikely to actually cause any problems.  A check is made in
 * new_rule() to ensure that this limit is not reached.
 */

/* Mask to mark a trailing context accepting number. */
#define YY_TRAILING_MASK 0x2000

/* Mask to mark the accepting number of the "head" of a trailing context
 * rule.
 */
#define YY_TRAILING_HEAD_MASK 0x4000

/* Maximum number of rules, as outlined in the above note. */
#define MAX_RULE (YY_TRAILING_MASK - 1)


/* NIL must be 0.  If not, its special meaning when making equivalence classes
 * (it marks the representative of a given e.c.) will be unidentifiable.
 */
#define NIL 0

#define JAM -1                  /* to mark a missing DFA transition */
#define NO_TRANSITION NIL
#define UNIQUE -1               /* marks a symbol as an e.c. representative */
#define INFINITY -1             /* for x{5,} constructions */

#define INITIAL_MAX_CCLS 100    /* max number of unique character classes */
#define MAX_CCLS_INCREMENT 100

/* Size of table holding members of character classes. */
#define INITIAL_MAX_CCL_TBL_SIZE 500
#define MAX_CCL_TBL_SIZE_INCREMENT 250

#define INITIAL_MAX_RULES 100   /* default maximum number of rules */
#define MAX_RULES_INCREMENT 100

#define INITIAL_MNS 2000        /* default maximum number of nfa states */
#define MNS_INCREMENT 1000      /* amount to bump above by if it's not enough */

#define INITIAL_MAX_DFAS 1000   /* default maximum number of dfa states */
#define MAX_DFAS_INCREMENT 1000

#define JAMSTATE -32766         /* marks a reference to the state that always jams */

/* Maximum number of NFA states. */
#define MAXIMUM_MNS 31999

/* Enough so that if it's subtracted from an NFA state number, the result
 * is guaranteed to be negative.
 */
#define MARKER_DIFFERENCE (MAXIMUM_MNS+2)

/* Maximum number of nxt/chk pairs for non-templates. */
#define INITIAL_MAX_XPAIRS 2000
#define MAX_XPAIRS_INCREMENT 2000

/* Maximum number of nxt/chk pairs needed for templates. */
#define INITIAL_MAX_TEMPLATE_XPAIRS 2500
#define MAX_TEMPLATE_XPAIRS_INCREMENT 2500

#define SYM_EPSILON (CSIZE + 1) /* to mark transitions on the symbol epsilon */

#define INITIAL_MAX_SCS 40      /* maximum number of start conditions */
#define MAX_SCS_INCREMENT 40    /* amount to bump by if it's not enough */

#define ONE_STACK_SIZE 500      /* stack of states with only one out-transition */
#define SAME_TRANS -1           /* transition is the same as "default" entry for state */


/* Size of region set aside to cache the complete transition table of
 * protos on the proto queue to enable quick comparisons.
 */
#define PROT_SAVE_SIZE 2000

#define MSP 50                  /* maximum number of saved protos (protos on the proto queue) */


#endif /* FLEXDEF_H */
