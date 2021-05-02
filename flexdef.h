/* flexdef - definitions file for flex */

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

#include "flexchar.h"           /* Char */

#include <stdio.h>              /* FILE, etc. */

/* As an aid for the internationalization patch to flex, which
 * is maintained outside this distribution for copyright reasons.
 */
#define _(String) (String)

/* Size of input alphabet - should be size of ASCII set. */
#ifndef DEFAULT_CSIZE
#  define DEFAULT_CSIZE 128
#endif

#ifdef VMS
#  ifndef __VMS_POSIX
#    define SHORT_FILE_NAMES
#  endif
#endif


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


/* Special chk[] values marking the slots taking by end-of-buffer and action
 * numbers.
 */
#define EOB_POSITION -1
#define ACTION_POSITION -2

/* Number of data items per line for -f output. */
#define NUMDATAITEMS 10

/* Number of lines of data in -f output before inserting a blank line for
 * readability.
 */
#define NUMDATALINES 10

/* transition_struct_out() definitions. */
#define TRANS_STRUCT_PRINT_LENGTH 14

/* Returns true if an nfa state has an epsilon out-transition slot
 * that can be used.  This definition is currently not used.
 */
#define FREE_EPSILON(state) \
        (transchar[state] == SYM_EPSILON && \
         trans2[state] == NO_TRANSITION && \
         finalst[state] != state)

/* Returns true if an nfa state has an epsilon out-transition character
 * and both slots are free
 */
#define SUPER_FREE_EPSILON(state) \
        (transchar[state] == SYM_EPSILON && \
         trans1[state] == NO_TRANSITION) \

/* Maximum number of NFA states that can comprise a DFA state.  It's real
 * big because if there's a lot of rules, the initial state will have a
 * huge epsilon closure.
 */
#define INITIAL_MAX_DFA_SIZE 750
#define MAX_DFA_SIZE_INCREMENT 750


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

/* The following percentages are used to tune table compression:

 * The percentage the number of out-transitions a state must be of the
 * number of equivalence classes in order to be considered for table
 * compaction by using protos.
 */
#define PROTO_SIZE_PERCENTAGE 15

/* The percentage the number of homogeneous out-transitions of a state
 * must be of the number of total out-transitions of the state in order
 * that the state's transition table is first compared with a potential
 * template of the most common out-transition instead of with the first
 * proto in the proto queue.
 */
#define CHECK_COM_PERCENTAGE 50

/* The percentage the number of differences between a state's transition
 * table and the proto it was first compared with must be of the total
 * number of out-transitions of the state in order to keep the first
 * proto as a good match and not search any further.
 */
#define FIRST_MATCH_DIFF_PERCENTAGE 10

/* The percentage the number of differences between a state's transition
 * table and the most similar proto must be of the state's total number
 * of out-transitions to use the proto as an acceptable close match.
 */
#define ACCEPTABLE_DIFF_PERCENTAGE 50

/* The percentage the number of homogeneous out-transitions of a state
 * must be of the number of total out-transitions of the state in order
 * to consider making a template from the state.
 */
#define TEMPLATE_SAME_PERCENTAGE 60

/* The percentage the number of differences between a state's transition
 * table and the most similar proto must be of the state's total number
 * of out-transitions to create a new proto from the state.
 */
#define NEW_PROTO_DIFF_PERCENTAGE 20

/* The percentage the total number of out-transitions of a state must be
 * of the number of equivalence classes in order to consider trying to
 * fit the transition table into "holes" inside the nxt/chk table.
 */
#define INTERIOR_FIT_PERCENTAGE 15

/* Size of region set aside to cache the complete transition table of
 * protos on the proto queue to enable quick comparisons.
 */
#define PROT_SAVE_SIZE 2000

#define MSP 50                  /* maximum number of saved protos (protos on the proto queue) */

/* Maximum number of out-transitions a state can have that we'll rummage
 * around through the interior of the internal fast table looking for a
 * spot for it.
 */
#define MAX_XTIONS_FULL_INTERIOR_FIT 4

/* Maximum number of rules which will be reported as being associated
 * with a DFA state.
 */
#define MAX_ASSOC_RULES 100

/* Number that, if used to subscript an array, has a good chance of producing
 * an error; should be small enough to fit into a short.
 */
#define BAD_SUBSCRIPT -32767

/* Absolute value of largest number that can be stored in a short, with a
 * bit of slop thrown in for general paranoia.
 */
#define MAX_SHORT 32700


/* Declarations for global variables. */

/* Variables for flags:
 * printstats - if true (-v), dump statistics
 * syntaxerror - true if a syntax error has been found
 * eofseen - true if we've seen an eof in the input file
 * ddebug - if true (-d), make a "debug" scanner
 * trace - if true (-T), trace processing
 * nowarn - if true (-w), do not generate warnings
 * spprdflt - if true (-s), suppress the default rule
 * interactive - if true (-I), generate an interactive scanner
 * caseins - if true (-i), generate a case-insensitive scanner
 * lex_compat - if true (-l), maximize compatibility with AT&T lex
 * do_yylineno - if true, generate code to maintain yylineno
 * useecs - if true (-Ce flag), use equivalence classes
 * fulltbl - if true (-Cf flag), don't compress the DFA state table
 * usemecs - if true (-Cm flag), use meta-equivalence classes
 * fullspd - if true (-F flag), use Jacobson method of table representation
 * gen_line_dirs - if true (i.e., no -L flag), generate #line directives
 * performance_report - if > 0 (i.e., -p flag), generate a report relating
 *   to scanner performance; if > 1 (-p -p), report on minor performance
 *   problems, too
 * backing_up_report - if true (i.e., -b flag), generate "lex.backup" file
 *   listing backing-up states
 * C_plus_plus - if true (i.e., -+ flag), generate a C++ scanner class;
 *   otherwise, a standard C scanner
 * long_align - if true (-Ca flag), favor long-word alignment.
 * use_read - if true (-f, -F, or -Cr) then use read() for scanner input;
 *   otherwise, use fread().
 * yytext_is_array - if true (i.e., %array directive), then declare
 *   yytext as a array instead of a character pointer.  Nice and inefficient.
 * do_yywrap - do yywrap() processing on EOF.  If false, EOF treated as
 *   "no more files".
 * csize - size of character set for the scanner we're generating;
 *   128 for 7-bit chars and 256 for 8-bit
 * yymore_used - if true, yymore() is used in input rules
 * reject - if true, generate back-up tables for REJECT macro
 * real_reject - if true, scanner really uses REJECT (as opposed to just
 *   having "reject" set for variable trailing context)
 * continued_action - true if this rule's action is to "fall through" to
 *   the next rule's action (i.e., the '|' action)
 * in_rule - true if we're inside an individual rule, false if not.
 * yymore_really_used - whether to treat yymore() as really used, regardless
 *   of what we think based on references to it in the user's actions.
 * reject_really_used - same for REJECT
 */

extern int printstats, syntaxerror, eofseen, ddebug, trace, nowarn, spprdflt;
extern int interactive, caseins, lex_compat, do_yylineno;
extern int useecs, fulltbl, usemecs, fullspd;
extern int gen_line_dirs, performance_report, backing_up_report;
extern int C_plus_plus, long_align, use_read, yytext_is_array, do_yywrap;
extern int csize;
extern int yymore_used, reject, real_reject, continued_action, in_rule;

extern int yymore_really_used, reject_really_used;


/* Variables used in the flex input routines:
 * datapos - characters on current output line
 * dataline - number of contiguous lines of data in current data
 *      statement.  Used to generate readable -f output
 * linenum - current input line number
 * out_linenum - current output line number
 * skelfile - the skeleton file
 * scanner_skl_contents - compiled-in skeleton array
 * scanner_skl_ind - index into "scanner_skl_contents" array, if skelfile is nil
 * yyin - input file
 * backing_up_file - file to summarize backing-up states to
 * infilename - name of input file
 * outfilename - name of output file
 * did_outfilename - whether outfilename was explicitly set
 * prefix - the prefix used for externally visible names ("yy" by default)
 * yyclass - yyFlexLexer subclass to use for YY_DECL
 * do_stdinit - whether to initialize yyin/yyout to stdin/stdout
 * input_files - array holding names of input files
 * num_input_files - size of input_files array
 * program_name - name with which program was invoked
 *
 * action_array - array to hold the rule actions
 * action_size - size of action_array
 * defs1_offset - index where the user's section 1 definitions start
 *      in action_array
 * prolog_offset - index where the prolog starts in action_array
 * action_offset - index where the non-prolog starts in action_array
 * action_index - index where the next action should go, with respect
 *      to "action_array"
 */

extern int datapos, dataline, linenum, out_linenum;
extern FILE *skelfile, *yyin, *backing_up_file;
extern const char *scanner_skl_contents[];
extern int scanner_skl_ind;
extern char *infilename, *outfilename;
extern int did_outfilename;
extern char *prefix, *yyclass;
extern int do_stdinit;
extern char **input_files;
extern int num_input_files;
extern char *program_name;

extern char *action_array;
extern int action_size;
extern int defs1_offset, prolog_offset, action_offset, action_index;

/* The file to which we are writing the C or C++ scanner implementation. */
extern FILE *scanner_c_file;

/* If true, use platform-native line endings; otherwise LF only. */
extern int write_native_line_endings;

/* Defined in header.c, this has the contents of the header to emit. */
extern char const *header_skl_contents[];

/* Variables for stack of states having only one out-transition:
 * onestate - state number
 * onesym - transition symbol
 * onenext - target state
 * onedef - default base entry
 * onesp - stack pointer
 */

extern int onestate[ONE_STACK_SIZE], onesym[ONE_STACK_SIZE];
extern int onenext[ONE_STACK_SIZE], onedef[ONE_STACK_SIZE], onesp;


#endif /* FLEXDEF_H */
