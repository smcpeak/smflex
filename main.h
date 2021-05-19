/* main.h */
/* Declarations for main.c. */
/* See COPYING for license terms. */

#ifndef MAIN_H
#define MAIN_H

#include "flexchar.h"                  /* Char */
#include "flexdef.h"                   /* MAXLINE, ONE_STACK_SIZE, etc. */

#include <stdio.h>                     /* FILE */


/* See definitions for documentation. */
void check_options(void);
void flexend(int exit_status);
void flexinit(int argc, char **argv);
void readin(void);
void set_prefix(char const *new_prefix);
void set_up_initial_allocations(void);
void usage(void);


/* Declarations for global variables. */

/* Variables for flags:
 * printstats - if true (-v), dump statistics
 * syntaxerror - true if a syntax error has been found
 * eofseen - true if we've seen an eof in the input file
 * option_debug - if true (-d), make a "debug" scanner
 * trace - if true (-T), trace processing
 * nowarn - if true (-w), do not generate warnings
 * option_suppress_default_rule - if true (-s), suppress the default rule
 * interactive - if true (-I), generate an interactive scanner
 * caseins - if true (-i), generate a case-insensitive scanner
 * option_yylineno - if true, generate code to maintain yylineno
 * useecs - if true (-Ce flag), use equivalence classes
 * fulltbl - if true (-Cf flag), don't compress the DFA state table
 * usemecs - if true (-Cm flag), use meta-equivalence classes
 * jacobson - if true (-CJ flag), use Jacobson method of table representation
 * gen_line_dirs - if true (i.e., no -L flag), generate #line directives
 * performance_report - if > 0 (i.e., -p flag), generate a report relating
 *   to scanner performance; if > 1 (-p -p), report on minor performance
 *   problems, too
 * backing_up_report - if true (i.e., -b flag), generate "lex.backup" file
 *   listing backing-up states
 * cpp_interface - if true (i.e., -+ flag), generate a C++ scanner class;
 *   otherwise, a standard C scanner
 * long_align - if true (-Ca flag), favor long-word alignment.
 * use_read - if true (-Cr) then use read() for scanner input;
 *   otherwise, use fread().
 * csize - size of character set for the scanner we're generating;
 *   128 for 7-bit chars and 256 for 8-bit
 * option_yymore - if true, yymore() is enabled
 * reject_used - if true, generate back-up tables for REJECT macro
 * real_reject - if true, scanner really uses REJECT (as opposed to just
 *   having "reject" set for variable trailing context)
 * continued_action - true if this rule's action is to "fall through" to
 *   the next rule's action (i.e., the '|' action)
 * in_rule - true if we're inside an individual rule, false if not.
 * reject_really_used - same for REJECT
 */

extern int printstats, syntaxerror, eofseen, option_debug, trace, nowarn;
extern int option_suppress_default_rule;
extern int interactive, caseins, option_yylineno;
extern int useecs, fulltbl, usemecs, jacobson;
extern int gen_line_dirs, performance_report, backing_up_report;
extern int cpp_interface, long_align, use_read;
extern int csize;
extern int option_yymore, reject_used, real_reject, continued_action, in_rule;
extern int reject_really_used;

/* Initially false, set to true by "%option stack".  This controls
 * whether we emit the functions for manipulating the state stack. */
extern int option_stack;

/* Initially false, each of these is set to true by their respective
 * %options, and controls emission of the respective functions. */
extern int option_main;
extern int option_yy_read_character;
extern int option_yy_scan_string;
extern int option_yy_scan_bytes;
extern int option_yy_scan_buffer;
extern int option_yy_top_state; /* Only effective if 'option_stack'. */
extern int option_yy_unread_character;

/* Initially false, set to true with "%option flex-compat".  This
 * activates some aliases for 'flex' compatibility. */
extern int option_flex_compat;


/* Variables used in the smflex input routines:
 * datapos - characters on current output line
 * dataline - number of contiguous lines of data in current data
 *      statement.  Used to generate readable -f output
 * linenum - current input line number
 * out_linenum - current output line number
 * scanner_skl_ind - index into "scanner_skl_contents" array
 * backing_up_file - file to summarize backing-up states to
 * infilename - name of input file
 * outfilename - name of output file
 * did_outfilename - whether outfilename was explicitly set
 * prefix - the prefix used for externally visible names ("yy" by default)
 * all_caps_prefix - same as 'prefix' but in all caps
 * option_yyclass - if not NULL, yyFlexLexer subclass to use for 'lex' method
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
extern int scanner_skl_ind;
extern FILE *backing_up_file;
extern char *infilename, *outfilename;
extern int did_outfilename;
extern char const *prefix;
extern char const *all_caps_prefix;
extern char *option_yyclass;
extern char *program_name;
extern char *action_array;
extern int action_size;
extern int defs1_offset, prolog_offset, action_offset, action_index;


/* This contains the skeleton of the scanner implementation to
 * generate.  It is defined in generated-scanner.skl.c. */
extern const char *scanner_skl_contents[];

/* This contains the skeleton of the C++ header file to generate when
 * generating a C++-interface scanner.  It is defined in
 * generated-header.skl.c. */
extern char const *header_skl_contents[];


/* The file to which we are writing the C or C++ scanner implementation. */
extern FILE *scanner_c_file;

/* If true, use platform-native line endings; otherwise LF only. */
extern int write_native_line_endings;


/* Variables for stack of states having only one out-transition:
 * onestate - state number
 * onesym - transition symbol
 * onenext - target state
 * onedef - default base entry
 * onesp - stack pointer
 */

extern int onestate[ONE_STACK_SIZE], onesym[ONE_STACK_SIZE];
extern int onenext[ONE_STACK_SIZE], onedef[ONE_STACK_SIZE], onesp;


/* Variables for nfa machine data:
 * current_mns - current maximum on number of NFA states
 * num_rules - number of the last accepting state; also is number of
 *      rules created so far
 * num_eof_rules - number of <<EOF>> rules
 * default_rule - number of the default rule
 * current_max_rules - current maximum number of rules
 * lastnfa - last nfa state number created
 * firstst - physically the first state of a fragment
 * lastst - last physical state of fragment
 * finalst - last logical state of fragment
 * transchar - transition character
 * trans1 - transition state
 * trans2 - 2nd transition state for epsilons
 * accptnum - accepting number
 * assoc_rule - rule associated with this NFA state (or 0 if none)
 * state_type - a STATE_xxx type identifying whether the state is part
 *      of a normal rule, the leading state in a trailing context
 *      rule (i.e., the state which marks the transition from
 *      recognizing the text-to-be-matched to the beginning of
 *      the trailing context), or a subsequent state in a trailing
 *      context rule
 * rule_type - a RULE_xxx type identifying whether this a ho-hum
 *      normal rule or one which has variable head & trailing
 *      context
 * rule_linenum - line number associated with rule
 * rule_useful - true if we've determined that the rule can be matched
 */

extern int current_mns, current_max_rules;
extern int num_rules, num_eof_rules, default_rule, lastnfa;
extern int *firstst, *lastst, *finalst, *transchar, *trans1, *trans2;
extern int *accptnum, *assoc_rule, *state_type;
extern int *rule_type, *rule_linenum, *rule_useful;

/* Different types of states; values are useful as masks, as well, for
 * routines like check_trailing_context().
 */
#define STATE_NORMAL 0x1
#define STATE_TRAILING_CONTEXT 0x2

/* Different types of rules. */
#define RULE_NORMAL 0
#define RULE_VARIABLE 1


/* Global holding current type of state we're making. */
extern int current_state_type;

/* True if the input rules include a rule with both variable-length head
 * and trailing context, false otherwise.
 */
extern int variable_trailing_context_rules;


/* Variables for protos:
 * numtemps - number of templates created
 * numprots - number of protos created
 * protprev - backlink to a more-recently used proto
 * protnext - forward link to a less-recently used proto
 * prottbl - base/def table entry for proto
 * protcomst - common state of proto
 * firstprot - number of the most recently used proto
 * lastprot - number of the least recently used proto
 * protsave contains the entire state array for protos
 */

extern int numtemps, numprots, protprev[MSP], protnext[MSP], prottbl[MSP];
extern int protcomst[MSP], firstprot, lastprot, protsave[PROT_SAVE_SIZE];


/* Variables for managing equivalence classes:
 * numecs - number of equivalence classes
 * nextecm - forward link of Equivalence Class members
 * ecgroup - class number or backward link of EC members
 * nummecs - number of meta-equivalence classes (used to compress
 *   templates)
 * tecfwd - forward link of meta-equivalence classes members
 * tecbck - backward link of MEC's
 */

/* Reserve enough room in the equivalence class arrays so that we
 * can use the CSIZE'th element to hold equivalence class information
 * for the NUL character.  Later we'll move this information into
 * the 0th element.
 */
extern int numecs, nextecm[CSIZE + 1], ecgroup[CSIZE + 1], nummecs;

/* Meta-equivalence classes are indexed starting at 1, so it's possible
 * that they will require positions from 1 .. CSIZE, i.e., CSIZE + 1
 * slots total (since the arrays are 0-based).  nextecm[] and ecgroup[]
 * don't require the extra position since they're indexed from 1 .. CSIZE - 1.
 */
extern int tecfwd[CSIZE + 1], tecbck[CSIZE + 1];


/* Variables for start conditions:
 * lastsc - last start condition created
 * current_max_scs - current limit on number of start conditions
 * scset - set of rules active in start condition
 * scbol - set of rules active only at the beginning of line in a s.c.
 * scxclu - true if start condition is exclusive
 * sceof - true if start condition has EOF rule
 * scname - start condition name
 */

extern int lastsc, *scset, *scbol, *scxclu, *sceof;
extern int current_max_scs;
extern char **scname;


/* Variables for dfa machine data:
 * current_max_dfa_size - current maximum number of NFA states in DFA
 * current_max_xpairs - current maximum number of non-template xtion pairs
 * current_max_template_xpairs - current maximum number of template pairs
 * current_max_dfas - current maximum number DFA states
 * lastdfa - last dfa state number created
 * nxt - state to enter upon reading character
 * chk - check value to see if "nxt" applies
 * tnxt - internal nxt table for templates
 * base - offset into "nxt" for given state
 * def - where to go if "chk" disallows "nxt" entry
 * nultrans - NUL transition for each state
 * NUL_ec - equivalence class of the NUL character
 * tblend - last "nxt/chk" table entry being used
 * firstfree - first empty entry in "nxt/chk" table
 * dss - nfa state set for each dfa
 * dfasiz - size of nfa state set for each dfa
 * dfaacc - accepting set for each dfa state (if using REJECT), or accepting
 *      number, if not
 * accsiz - size of accepting set for each dfa state
 * dhash - dfa state hash value
 * numas - number of DFA accepting states created; note that this
 *      is not necessarily the same value as num_rules, which is the analogous
 *      value for the NFA
 * numsnpairs - number of state/nextstate transition pairs
 * jambase - position in base/def where the default jam table starts
 * jamstate - state number corresponding to "jam" state
 * end_of_buffer_state - end-of-buffer dfa state number
 */

extern int current_max_dfa_size, current_max_xpairs;
extern int current_max_template_xpairs, current_max_dfas;
extern int lastdfa, *nxt, *chk, *tnxt;
extern int *base, *def, *nultrans, NUL_ec, tblend, firstfree, **dss, *dfasiz;
extern union dfaacc_union {
  int *dfaacc_set;
  int dfaacc_state;
} *dfaacc;
extern int *accsiz, *dhash, numas;
extern int numsnpairs, jambase, jamstate;
extern int end_of_buffer_state;


/* Variables for ccl information:
 * lastccl - ccl index of the last created ccl
 * current_maxccls - current limit on the maximum number of unique ccl's
 * cclmap - maps a ccl index to its set pointer
 * ccllen - gives the length of a ccl
 * cclng - true for a given ccl if the ccl is negated
 * cclreuse - counts how many times a ccl is re-used
 * current_max_ccl_tbl_size - current limit on number of characters needed
 *      to represent the unique ccl's
 * ccltbl - holds the characters in each ccl - indexed by cclmap
 */

extern int lastccl, *cclmap, *ccllen, *cclng, cclreuse;
extern int current_maxccls, current_max_ccl_tbl_size;
extern Char *ccltbl;


/* Variables for miscellaneous information:
 * nmstr - last NAME scanned by the scanner
 * sectnum - section number currently being parsed
 * nummt - number of empty nxt/chk table entries
 * hshcol - number of hash collisions detected by snstods
 * dfaeql - number of times a newly created dfa was equal to an old one
 * numeps - number of epsilon NFA states created
 * eps2 - number of epsilon states which have 2 out-transitions
 * num_reallocs - number of times it was necessary to realloc() a group
 *        of arrays
 * tmpuses - number of DFA states that chain to templates
 * totnst - total number of NFA states used to make DFA states
 * peakpairs - peak number of transition pairs we had to store internally
 * numuniq - number of unique transitions
 * numdup - number of duplicate transitions
 * hshsave - number of hash collisions saved by checking number of states
 * num_backing_up - number of DFA states requiring backing up
 * bol_needed - whether scanner needs beginning-of-line recognition
 */

extern char nmstr[MAXLINE];
extern int sectnum, nummt, hshcol, dfaeql, numeps, eps2, num_reallocs;
extern int tmpuses, totnst, peakpairs, numuniq, numdup, hshsave;
extern int num_backing_up, bol_needed;


/* Name of the C++ scanner header file. */
extern char header_file_name[MAXLINE];


#endif /* MAIN_H */
