/* gen - actual generation (writing) of smflex scanners */

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

#include "gen.h"                       /* this module */

#include "dfa.h"                       /* increase_max_dfas */
#include "input-scan.h"                /* flexscan */
#include "main.h"                      /* num_backing_up, etc. */
#include "misc.h"                      /* outc, outn, etc. */
#include "tblcmp.h"                    /* expand_nxt_chk */

#include <errno.h>                     /* errno */
#include <stdlib.h>                    /* exit */
#include <string.h>                    /* strerror, strcpy, strrchr */


/* Special chk[] values marking the slots taking by end-of-buffer and action
 * numbers.
 */
#define EOB_POSITION -1
#define ACTION_POSITION -2


/* Absolute value of largest number that can be stored in a short, with a
 * bit of slop thrown in for general paranoia.
 */
#define MAX_SHORT 32700


static int indent_level = 0;    /* each level is 8 spaces */

#define indent_up() (++indent_level)
#define indent_down() (--indent_level)
#define set_indent(indent_val) indent_level = indent_val

/* Almost everything is done in terms of arrays starting at 1, so provide
 * a null entry for the zero element of all C arrays.  (The exception
 * to this is that the fast table representation generally uses the
 * 0 elements of its arrays, too.)
 */
static char C_int_decl[] = "static const int %s[%d] =\n    {   0,\n";
static char C_short_decl[] =
  "static const short int %s[%d] =\n    {   0,\n";
static char C_long_decl[] = "static const long int %s[%d] =\n    {   0,\n";
static char C_state_decl[] =
  "static const yy_state_type %s[%d] =\n    {   0,\n";


/* Indent to the current level. */
void do_indent()
{
  int i = indent_level * 2;

  while (i > 0) {
    outc(' ');
    --i;
  }
}


/* Print an open brace on a line by itself, then increase the
 * indentation level. */
static void indent_lbrace()
{
  indent_puts("{");
  indent_up();
}


/* Decreas the indentation level, then put a close brace on a line
 * by itself. */
static void indent_rbrace()
{
  indent_down();
  indent_puts("}");
}


/* Generate the code to keep backing-up information. */
void gen_backing_up()
{
  if (reject || num_backing_up == 0)
    return;

  if (fullspd)
    indent_puts("if ( yy_current_state[-1].yy_nxt )");
  else
    indent_puts("if ( yy_accept[yy_current_state] )");

  indent_lbrace();
  indent_puts("yy_last_accepting_state = yy_current_state;");
  indent_puts("yy_last_accepting_cpos = yy_cp;");
  indent_rbrace();
}


/* Generate the code to perform the backing up. */
void gen_bu_action()
{
  if (reject || num_backing_up == 0)
    return;

  set_indent(3);

  indent_puts("case 0: /* must back up */");
  indent_puts("/* undo the effects of YY_DO_BEFORE_ACTION */");
  indent_puts("*yy_cp = yy_hold_char;");

  if (fullspd || fulltbl)
    indent_puts("yy_cp = yy_last_accepting_cpos + 1;");
  else
    /* Backing-up info for compressed tables is taken \after/
     * yy_cp has been incremented for the next state.
     */
    indent_puts("yy_cp = yy_last_accepting_cpos;");

  indent_puts("yy_current_state = yy_last_accepting_state;");
  indent_puts("goto yy_find_action;");
  outc('\n');

  set_indent(0);
}


/* genctbl - generates full speed compressed transition table */
void genctbl()
{
  int i;
  int end_of_buffer_action = num_rules + 1;

  /* Table of verify for transition and offset to next state. */
  out_dec("static const struct yy_trans_info yy_transition[%d] =\n",
          tblend + numecs + 1);
  outn("    {");

  /* We want the transition to be represented as the offset to the
   * next state, not the actual state number, which is what it currently
   * is.  The offset is base[nxt[i]] - (base of current state)].  That's
   * just the difference between the starting points of the two involved
   * states (to - from).
   *
   * First, though, we need to find some way to put in our end-of-buffer
   * flags and states.  We do this by making a state with absolutely no
   * transitions.  We put it at the end of the table.
   */

  /* We need to have room in nxt/chk for two more slots: One for the
   * action and one for the end-of-buffer transition.  We now *assume*
   * that we're guaranteed the only character we'll try to index this
   * nxt/chk pair with is EOB, i.e., 0, so we don't have to make sure
   * there's room for jam entries for other characters.
   */

  while (tblend + 2 >= current_max_xpairs)
    expand_nxt_chk();

  while (lastdfa + 1 >= current_max_dfas)
    increase_max_dfas();

  base[lastdfa + 1] = tblend + 2;
  nxt[tblend + 1] = end_of_buffer_action;
  chk[tblend + 1] = numecs + 1;
  chk[tblend + 2] = 1;          /* anything but EOB */

  /* So that "make test" won't show arb. differences. */
  nxt[tblend + 2] = 0;

  /* Make sure every state has an end-of-buffer transition and an
   * action #.
   */
  for (i = 0; i <= lastdfa; ++i) {
    int anum = dfaacc[i].dfaacc_state;
    int offset = base[i];

    chk[offset] = EOB_POSITION;
    chk[offset - 1] = ACTION_POSITION;
    nxt[offset - 1] = anum;     /* action number */
  }

  for (i = 0; i <= tblend; ++i) {
    if (chk[i] == EOB_POSITION)
      transition_struct_out(0, base[lastdfa + 1] - i);

    else if (chk[i] == ACTION_POSITION)
      transition_struct_out(0, nxt[i]);

    else if (chk[i] > numecs || chk[i] == 0)
      transition_struct_out(0, 0);      /* unused slot */

    else                        /* verify, transition */
      transition_struct_out(chk[i], base[nxt[i]] - (i - chk[i]));
  }


  /* Here's the final, end-of-buffer state. */
  transition_struct_out(chk[tblend + 1], nxt[tblend + 1]);
  transition_struct_out(chk[tblend + 2], nxt[tblend + 2]);

  outn("    };\n");

  /* Table of pointers to start states. */
  out_dec("static const struct yy_trans_info *yy_start_state_list[%d] =\n",
          lastsc * 2 + 1);
  outn("    {");                /* } so vi doesn't get confused */

  for (i = 0; i <= lastsc * 2; ++i)
    out_dec("    &yy_transition[%d],\n", base[i]);

  dataend();

  if (useecs)
    genecs();
}


/* Generate equivalence-class tables. */
void genecs()
{
  int i, j;
  int numrows;

  out_str_dec(C_int_decl, "yy_ec", csize);

  for (i = 1; i < csize; ++i) {
    if (caseins && (i >= 'A') && (i <= 'Z'))
      ecgroup[i] = ecgroup[clower(i)];

    ecgroup[i] = ABS(ecgroup[i]);
    mkdata(ecgroup[i]);
  }

  dataend();

  if (trace) {
    fputs(_("\n\nEquivalence Classes:\n\n"), stderr);

    numrows = csize / 8;

    for (j = 0; j < numrows; ++j) {
      for (i = j; i < csize; i = i + numrows) {
        fprintf(stderr, "%4s = %-2d", readable_form(i), ecgroup[i]);

        putc(' ', stderr);
      }

      putc('\n', stderr);
    }
  }
}


/* Generate the code to find the action number. */
void gen_find_action()
{
  if (fullspd)
    indent_puts("yy_act = yy_current_state[-1].yy_nxt;");

  else if (fulltbl)
    indent_puts("yy_act = yy_accept[yy_current_state];");

  else if (reject) {
    indent_puts("yy_current_state = *--yy_state_ptr;");
    indent_puts("yy_lp = yy_accept[yy_current_state];");

    /* One way this is unused is '%option yylineno', which claims
     * to use REJECT (presumably because it needs similar table
     * maintenance) but actually does not. */
    outn("YY_POSSIBLY_UNUSED_LABEL(find_rule) /* we branch to this label when backing up */");

    indent_puts("for ( ; ; ) /* until we find what rule we matched */");

    indent_lbrace();

    indent_puts("if ( yy_lp && yy_lp < yy_accept[yy_current_state + 1] )");
    indent_lbrace();
    indent_puts("yy_act = yy_acclist[yy_lp];");

    if (variable_trailing_context_rules) {
      indent_puts("if ( yy_act & YY_TRAILING_HEAD_MASK ||");
      indent_puts("     yy_looking_for_trail_begin )");
      indent_lbrace();

      indent_puts("if ( yy_act == yy_looking_for_trail_begin )");
      indent_lbrace();
      indent_puts("yy_looking_for_trail_begin = 0;");
      indent_puts("yy_act &= ~YY_TRAILING_HEAD_MASK;");
      indent_puts("break;");
      indent_rbrace();

      indent_rbrace();

      indent_puts("else if ( yy_act & YY_TRAILING_MASK )");
      indent_lbrace();
      indent_puts("yy_looking_for_trail_begin = yy_act & ~YY_TRAILING_MASK;");
      indent_puts("yy_looking_for_trail_begin |= YY_TRAILING_HEAD_MASK;");

      if (real_reject) {
        /* Remember matched text in case we back up
         * due to REJECT.
         */
        indent_puts("yy_full_match = yy_cp;");
        indent_puts("yy_full_state = yy_state_ptr;");
        indent_puts("yy_full_lp = yy_lp;");
      }

      indent_rbrace();

      indent_puts("else");
      indent_lbrace();
      indent_puts("yy_full_match = yy_cp;");
      indent_puts("yy_full_state = yy_state_ptr;");
      indent_puts("yy_full_lp = yy_lp;");
      indent_puts("break;");
      indent_rbrace();

      indent_puts("++yy_lp;");
      indent_puts("goto find_rule;");
    }

    else {
      /* Remember matched text in case we back up due to
       * trailing context plus REJECT.
       */
      indent_lbrace();
      indent_puts("yy_full_match = yy_cp;");
      indent_puts("break;");
      indent_rbrace();
    }

    indent_rbrace();

    indent_puts("--yy_cp;");

    /* We could consolidate the following two lines with those at
     * the beginning, but at the cost of complaints that we're
     * branching inside a loop.
     */
    indent_puts("yy_current_state = *--yy_state_ptr;");
    indent_puts("yy_lp = yy_accept[yy_current_state];");

    indent_rbrace();
  }

  else {                        /* compressed */
    indent_puts("yy_act = yy_accept[yy_current_state];");

    if (interactive && !reject) {
      /* Do the guaranteed-needed backing up to figure out
       * the match.
       */
      indent_puts("if ( yy_act == 0 )");
      indent_up();
      indent_puts("{ /* have to back up */");
      indent_puts("yy_cp = yy_last_accepting_cpos;");
      indent_puts("yy_current_state = yy_last_accepting_state;");
      indent_puts("yy_act = yy_accept[yy_current_state];");
      indent_rbrace();
    }
  }
}


/* genftbl - generate full transition table */
void genftbl()
{
  int i;
  int end_of_buffer_action = num_rules + 1;

  out_str_dec(long_align ? C_long_decl : C_short_decl,
              "yy_accept", lastdfa + 1);

  dfaacc[end_of_buffer_state].dfaacc_state = end_of_buffer_action;

  for (i = 1; i <= lastdfa; ++i) {
    int anum = dfaacc[i].dfaacc_state;

    mkdata(anum);

    if (trace && anum)
      fprintf(stderr, _("state # %d accepts: [%d]\n"), i, anum);
  }

  dataend();

  if (useecs)
    genecs();

  /* Don't have to dump the actual full table entries - they were
   * created on-the-fly.
   */
}


/* Generate the code to find the next compressed-table state. */
void gen_next_compressed_state(char *char_map)
{
  indent_put2s("YY_CHAR yy_c = %s;", char_map);

  /* Save the backing-up info \before/ computing the next state
   * because we always compute one more state than needed - we
   * always proceed until we reach a jam state
   */
  gen_backing_up();

  indent_puts
    ("while ( yy_chk[yy_base[yy_current_state] + yy_c] != yy_current_state )");
  indent_lbrace();
  indent_puts("yy_current_state = (int) yy_def[yy_current_state];");

  if (usemecs) {
    /* We've arrange it so that templates are never chained
     * to one another.  This means we can afford to make a
     * very simple test to see if we need to convert to
     * yy_c's meta-equivalence class without worrying
     * about erroneously looking up the meta-equivalence
     * class twice
     */
    do_indent();

    /* lastdfa + 2 is the beginning of the templates */
    out_dec("if ( yy_current_state >= %d )\n", lastdfa + 2);

    indent_up();
    indent_puts("yy_c = yy_meta[(unsigned int) yy_c];");
    indent_down();
  }

  indent_rbrace();

  indent_puts
    ("yy_current_state = yy_nxt[yy_base[yy_current_state] + (unsigned int) yy_c];");
}


/* Generate the code to find the next match. */
void gen_next_match()
{
  /* NOTE - changes in here should be reflected in gen_next_state() and
   * gen_NUL_trans().
   */
  char *char_map = useecs ?
    "yy_ec[YY_SC_TO_UI(*yy_cp)]" : "YY_SC_TO_UI(*yy_cp)";

  char *char_map_2 = useecs ?
    "yy_ec[YY_SC_TO_UI(*++yy_cp)]" : "YY_SC_TO_UI(*++yy_cp)";

  if (fulltbl) {
    indent_put2s
      ("while ( (yy_current_state = yy_nxt[yy_current_state][%s]) > 0 )",
       char_map);

    indent_lbrace();

    if (num_backing_up > 0) {
      gen_backing_up();
      outc('\n');
    }

    indent_puts("++yy_cp;");

    indent_rbrace();

    outc('\n');
    indent_puts("yy_current_state = -yy_current_state;");
  }

  else if (fullspd) {
    indent_lbrace();
    indent_puts("const struct yy_trans_info *yy_trans_info;\n");
    indent_puts("YY_CHAR yy_c;\n");
    indent_put2s("for ( yy_c = %s;", char_map);
    indent_puts
      ("      (yy_trans_info = &yy_current_state[(unsigned int) yy_c])->");
    indent_puts("yy_verify == yy_c;");
    indent_put2s("      yy_c = %s )", char_map_2);

    indent_lbrace();

    indent_puts("yy_current_state += yy_trans_info->yy_nxt;");

    if (num_backing_up > 0) {
      outc('\n');
      gen_backing_up();
    }

    indent_rbrace();

    indent_rbrace();
  }

  else {                        /* compressed */
    indent_puts("do");

    indent_lbrace();

    gen_next_state(false);

    indent_puts("++yy_cp;");

    indent_rbrace();

    do_indent();

    if (interactive)
      out_dec("while ( yy_base[yy_current_state] != %d );\n", jambase);
    else
      out_dec("while ( yy_current_state != %d );\n", jamstate);

    if (!reject && !interactive) {
      /* Do the guaranteed-needed backing up to figure out
       * the match.
       */
      indent_puts("yy_cp = yy_last_accepting_cpos;");
      indent_puts("yy_current_state = yy_last_accepting_state;");
    }
  }
}


/* Generate the code to find the next state. */
void gen_next_state(int worry_about_NULs)
{                               /* NOTE - changes in here should be reflected in gen_next_match() */
  char char_map[256];

  if (worry_about_NULs && !nultrans) {
    if (useecs)
      (void) sprintf(char_map,
                     "(*yy_cp ? yy_ec[YY_SC_TO_UI(*yy_cp)] : %d)", NUL_ec);
    else
      (void) sprintf(char_map, "(*yy_cp ? YY_SC_TO_UI(*yy_cp) : %d)", NUL_ec);
  }

  else
    strcpy(char_map, useecs ?
           "yy_ec[YY_SC_TO_UI(*yy_cp)]" : "YY_SC_TO_UI(*yy_cp)");

  if (worry_about_NULs && nultrans) {
    if (!fulltbl && !fullspd)
      /* Compressed tables back up *before* they match. */
      gen_backing_up();

    indent_puts("if ( *yy_cp )");
    indent_lbrace();
  }

  if (fulltbl)
    indent_put2s("yy_current_state = yy_nxt[yy_current_state][%s];",
                 char_map);

  else if (fullspd)
    indent_put2s("yy_current_state += yy_current_state[%s].yy_nxt;",
                 char_map);

  else
    gen_next_compressed_state(char_map);

  if (worry_about_NULs && nultrans) {
    indent_rbrace();
    indent_puts("else");
    indent_up();
    indent_puts("yy_current_state = yy_NUL_trans[yy_current_state];");
    indent_down();
  }

  if (fullspd || fulltbl)
    gen_backing_up();

  if (reject)
    indent_puts("*yy_state_ptr++ = yy_current_state;");
}


/* Generate the code to make a NUL transition. */
void gen_NUL_trans()
{                               /* NOTE - changes in here should be reflected in gen_next_match() */
  /* Only generate a definition for "yy_cp" if we'll generate code
   * that uses it.  Otherwise lint and the like complain.
   */
  int need_backing_up = (num_backing_up > 0 && !reject);

  if (need_backing_up && (!nultrans || fullspd || fulltbl))
    /* We're going to need yy_cp lying around for the call
     * below to gen_backing_up().
     */
    indent_puts("char *yy_cp = yy_c_buf_p;");

  outc('\n');

  if (nultrans) {
    indent_puts("yy_current_state = yy_NUL_trans[yy_current_state];");
    indent_puts("yy_is_jam = (yy_current_state == 0);");
  }

  else if (fulltbl) {
    do_indent();
    out_dec("yy_current_state = yy_nxt[yy_current_state][%d];\n", NUL_ec);
    indent_puts("yy_is_jam = (yy_current_state <= 0);");
  }

  else if (fullspd) {
    do_indent();
    out_dec("int yy_c = %d;\n", NUL_ec);

    indent_puts("const struct yy_trans_info *yy_trans_info;\n");
    indent_puts("yy_trans_info = &yy_current_state[(unsigned int) yy_c];");
    indent_puts("yy_current_state += yy_trans_info->yy_nxt;");

    indent_puts("yy_is_jam = (yy_trans_info->yy_verify != yy_c);");
  }

  else {
    char NUL_ec_str[20];

    (void) sprintf(NUL_ec_str, "%d", NUL_ec);
    gen_next_compressed_state(NUL_ec_str);

    do_indent();
    out_dec("yy_is_jam = (yy_current_state == %d);\n", jamstate);

    if (reject) {
      /* Only stack this state if it's a transition we
       * actually make.  If we stack it on a jam, then
       * the state stack and yy_c_buf_p get out of sync.
       */
      indent_puts("if ( ! yy_is_jam )");
      indent_up();
      indent_puts("*yy_state_ptr++ = yy_current_state;");
      indent_down();
    }
  }

  /* If we've entered an accepting state, back up; note that
   * compressed tables have *already* done such backing up, so
   * we needn't bother with it again.
   */
  if (need_backing_up && (fullspd || fulltbl)) {
    outc('\n');
    indent_puts("if ( ! yy_is_jam )");
    indent_lbrace();
    gen_backing_up();
    indent_rbrace();
  }
}


/* Generate the code to find the start state. */
void gen_start_state()
{
  if (fullspd) {
    if (bol_needed) {
      indent_puts
        ("yy_current_state = yy_start_state_list[yy_start + YY_AT_BOL()];");
    }
    else
      indent_puts("yy_current_state = yy_start_state_list[yy_start];");
  }

  else {
    indent_puts("yy_current_state = yy_start;");

    if (bol_needed)
      indent_puts("yy_current_state += YY_AT_BOL();");

    if (reject) {
      /* Set up for storing up states. */
      indent_puts("yy_state_ptr = yy_state_buf;");
      indent_puts("*yy_state_ptr++ = yy_current_state;");
    }
  }
}


/* gentabs - generate data statements for the transition tables */
void gentabs()
{
  int i, j, k, *accset, nacc, *acc_array, total_states;
  int end_of_buffer_action = num_rules + 1;

  acc_array = allocate_integer_array(current_max_dfas);
  nummt = 0;

  /* The compressed table format jams by entering the "jam state",
   * losing information about the previous state in the process.
   * In order to recover the previous state, we effectively need
   * to keep backing-up information.
   */
  ++num_backing_up;

  if (reject) {
    /* Write out accepting list and pointer list.
     *
     * First we generate the "yy_acclist" array.  In the process,
     * we compute the indices that will go into the "yy_accept"
     * array, and save the indices in the dfaacc array.
     */
    int EOB_accepting_list[2];

    /* Set up accepting structures for the End Of Buffer state. */
    EOB_accepting_list[0] = 0;
    EOB_accepting_list[1] = end_of_buffer_action;
    accsiz[end_of_buffer_state] = 1;
    dfaacc[end_of_buffer_state].dfaacc_set = EOB_accepting_list;

    out_str_dec(long_align ? C_long_decl : C_short_decl,
                "yy_acclist", MAX(numas, 1) + 1);

    j = 1;                      /* index into "yy_acclist" array */

    for (i = 1; i <= lastdfa; ++i) {
      acc_array[i] = j;

      if (accsiz[i] != 0) {
        accset = dfaacc[i].dfaacc_set;
        nacc = accsiz[i];

        if (trace)
          fprintf(stderr, _("state # %d accepts: "), i);

        for (k = 1; k <= nacc; ++k) {
          int accnum = accset[k];

          ++j;

          if (variable_trailing_context_rules &&
              !(accnum & YY_TRAILING_HEAD_MASK) &&
              accnum > 0 && accnum <= num_rules &&
              rule_type[accnum] == RULE_VARIABLE) {
            /* Special hack to flag
             * accepting number as part
             * of trailing context rule.
             */
            accnum |= YY_TRAILING_MASK;
          }

          mkdata(accnum);

          if (trace) {
            fprintf(stderr, "[%d]", accset[k]);

            if (k < nacc)
              fputs(", ", stderr);
            else
              putc('\n', stderr);
          }
        }
      }
    }

    /* add accepting number for the "jam" state */
    acc_array[i] = j;

    dataend();
  }

  else {
    dfaacc[end_of_buffer_state].dfaacc_state = end_of_buffer_action;

    for (i = 1; i <= lastdfa; ++i)
      acc_array[i] = dfaacc[i].dfaacc_state;

    /* add accepting number for jam state */
    acc_array[i] = 0;
  }

  /* Spit out "yy_accept" array.  If we're doing "reject", it'll be
   * pointers into the "yy_acclist" array.  Otherwise it's actual
   * accepting numbers.  In either case, we just dump the numbers.
   */

  /* "lastdfa + 2" is the size of "yy_accept"; includes room for C arrays
   * beginning at 0 and for "jam" state.
   */
  k = lastdfa + 2;

  if (reject)
    /* We put a "cap" on the table associating lists of accepting
     * numbers with state numbers.  This is needed because we tell
     * where the end of an accepting list is by looking at where
     * the list for the next state starts.
     */
    ++k;

  out_str_dec(long_align ? C_long_decl : C_short_decl, "yy_accept", k);

  for (i = 1; i <= lastdfa; ++i) {
    mkdata(acc_array[i]);

    if (!reject && trace && acc_array[i])
      fprintf(stderr, _("state # %d accepts: [%d]\n"), i, acc_array[i]);
  }

  /* Add entry for "jam" state. */
  mkdata(acc_array[i]);

  if (reject)
    /* Add "cap" for the list. */
    mkdata(acc_array[i]);

  dataend();

  if (useecs)
    genecs();

  if (usemecs) {
    /* Write out meta-equivalence classes (used to index
     * templates with).
     */

    if (trace)
      fputs(_("\n\nMeta-Equivalence Classes:\n"), stderr);

    out_str_dec(C_int_decl, "yy_meta", numecs + 1);

    for (i = 1; i <= numecs; ++i) {
      if (trace)
        fprintf(stderr, "%d = %d\n", i, ABS(tecbck[i]));

      mkdata(ABS(tecbck[i]));
    }

    dataend();
  }

  total_states = lastdfa + numtemps;

  out_str_dec((tblend >= MAX_SHORT || long_align) ?
              C_long_decl : C_short_decl, "yy_base", total_states + 1);

  for (i = 1; i <= lastdfa; ++i) {
    int d = def[i];

    if (base[i] == JAMSTATE)
      base[i] = jambase;

    if (d == JAMSTATE)
      def[i] = jamstate;

    else if (d < 0) {
      /* Template reference. */
      ++tmpuses;
      def[i] = lastdfa - d + 1;
    }

    mkdata(base[i]);
  }

  /* Generate jam state's base index. */
  mkdata(base[i]);

  for (++i /* skip jam state */ ; i <= total_states; ++i) {
    mkdata(base[i]);
    def[i] = jamstate;
  }

  dataend();

  out_str_dec((total_states >= MAX_SHORT || long_align) ?
              C_long_decl : C_short_decl, "yy_def", total_states + 1);

  for (i = 1; i <= total_states; ++i)
    mkdata(def[i]);

  dataend();

  out_str_dec((total_states >= MAX_SHORT || long_align) ?
              C_long_decl : C_short_decl, "yy_nxt", tblend + 1);

  for (i = 1; i <= tblend; ++i) {
    /* Note, the order of the following test is important.
     * If chk[i] is 0, then nxt[i] is undefined.
     */
    if (chk[i] == 0 || nxt[i] == 0)
      nxt[i] = jamstate;        /* new state is the JAM state */

    mkdata(nxt[i]);
  }

  dataend();

  out_str_dec((total_states >= MAX_SHORT || long_align) ?
              C_long_decl : C_short_decl, "yy_chk", tblend + 1);

  for (i = 1; i <= tblend; ++i) {
    if (chk[i] == 0)
      ++nummt;

    mkdata(chk[i]);
  }

  dataend();
}


/* Write out a formatted string (with a secondary string argument) at the
 * current indentation level, adding a final newline.
 */
void indent_put2s(char fmt[], char arg[])
{
  do_indent();
  out_str(fmt, arg);
  outn("");
}


/* Write out a string at the current indentation level, adding a final
 * newline.
 */
void indent_puts(char str[])
{
  do_indent();
  outn(str);
}


/* Call 'out' with 'text', except wherever it contains the substring
 * 'yy_output_file_line_directive', emit a #line directive that refers
 * to the output file. */
static void out_with_line_directive_substitution(char const *text)
{
  char const *marker;
  while ((marker = strstr(text, yy_output_file_line_directive)) != NULL) {
    /* Emit everything up to the marker. */
    out_with_limit(text, marker-text);

    /* Emit a real #line directive. */
    line_directive_out(scanner_c_file, 0 /*do_infile*/);

    /* Continue after the marker. */
    text = marker + strlen(yy_output_file_line_directive);
  }

  /* Emit the remainder. */
  out(text);
}


/* make_tables - generate transition tables and finishes generating output file
 */
void make_tables()
{
  int i;
  int did_eof_rule = false;

  skelout();

  /* First, take care of YY_DO_BEFORE_ACTION depending on yymore
   * being used.
   */
  set_indent(1);

  if (yymore_used) {
    indent_puts("yytext_ptr -= yy_more_len; \\");
    indent_puts("yyleng = (int) (yy_cp - yytext_ptr); \\");
  }

  else
    indent_puts("yyleng = (int) (yy_cp - yy_bp); \\");

  /* Now also deal with copying yytext_ptr to yytext if needed. */
  skelout();

  set_indent(0);

  skelout();


  out_dec("#define YY_NUM_RULES %d\n", num_rules);
  out_dec("#define YY_END_OF_BUFFER %d\n", num_rules + 1);

  if (fullspd) {
    /* Need to define the transet type as a size large
     * enough to hold the biggest offset.
     */
    int total_table_size = tblend + numecs + 1;
    char *trans_offset_type =
      (total_table_size >= MAX_SHORT || long_align) ? "long" : "short";

    set_indent(0);
    indent_puts("struct yy_trans_info");
    indent_lbrace();

    if (long_align)
      indent_puts("long yy_verify;");
    else
      indent_puts("short yy_verify;");

    /* In cases where its sister yy_verify *is* a "yes, there is
     * a transition", yy_nxt is the offset (in records) to the
     * next state.  In most cases where there is no transition,
     * the value of yy_nxt is irrelevant.  If yy_nxt is the -1th
     * record of a state, though, then yy_nxt is the action number
     * for that state.
     */

    indent_put2s("%s yy_nxt;", trans_offset_type);
    indent_puts("};");
    indent_down();
  }

  if (fullspd)
    genctbl();
  else if (fulltbl)
    genftbl();
  else
    gentabs();

  /* Definitions for backing up.  We don't need them if REJECT
   * is being used because then we use an alternative backin-up
   * technique instead.
   */
  if (num_backing_up > 0 && !reject) {
    if (!C_plus_plus) {
      indent_puts("static yy_state_type yy_last_accepting_state;");
      indent_puts("static char *yy_last_accepting_cpos;\n");
    }
  }

  if (nultrans) {
    out_str_dec(C_state_decl, "yy_NUL_trans", lastdfa + 1);

    for (i = 1; i <= lastdfa; ++i) {
      if (fullspd)
        out_dec("    &yy_transition[%d],\n", base[i]);
      else
        mkdata(nultrans[i]);
    }

    dataend();
  }

  if (ddebug) {                 /* Spit out table mapping rules to line numbers. */
    if (!C_plus_plus) {
      indent_puts("extern int yy_flex_debug;");
      indent_puts("int yy_flex_debug = 1;\n");
    }

    out_str_dec(long_align ? C_long_decl : C_short_decl,
                "yy_rule_linenum", num_rules);
    for (i = 1; i < num_rules; ++i)
      mkdata(rule_linenum[i]);
    dataend();
  }

  if (reject) {
    /* Declare state buffer variables. */
    if (!C_plus_plus) {
      outn
        ("static yy_state_type yy_state_buf[YY_BUF_SIZE + 2], *yy_state_ptr;");
      outn("static char *yy_full_match;");
      outn("static int yy_lp;");
    }

    if (variable_trailing_context_rules) {
      if (!C_plus_plus) {
        outn("static int yy_looking_for_trail_begin = 0;");
        outn("static int yy_full_lp;");
        outn("static int *yy_full_state;");
      }

      out_hex("#define YY_TRAILING_MASK 0x%x\n",
              (unsigned int) YY_TRAILING_MASK);
      out_hex("#define YY_TRAILING_HEAD_MASK 0x%x\n",
              (unsigned int) YY_TRAILING_HEAD_MASK);
    }

    outn("#define REJECT \\");
    outn("{ \\");
    outn("*yy_cp = yy_hold_char; /* undo effects of setting up yytext */ \\");
    outn("yy_cp = yy_full_match; /* restore poss. backed-over text */ \\");

    if (variable_trailing_context_rules) {
      outn("yy_lp = yy_full_lp; /* restore orig. accepting pos. */ \\");
      outn("yy_state_ptr = yy_full_state; /* restore orig. state */ \\");
      outn("yy_current_state = *yy_state_ptr; /* restore curr. state */ \\");
    }

    outn("++yy_lp; \\");
    outn("goto find_rule; \\");
    outn("}");
  }

  else {
    outn("/* The intent behind this definition is that it'll catch");
    outn(" * any uses of REJECT which smflex missed.");
    outn(" */");
    outn("#define REJECT reject_used_but_not_detected");
  }

  if (yymore_used) {
    if (!C_plus_plus) {
      indent_puts("static int yy_more_flag = 0;");
      indent_puts("static int yy_more_len = 0;");
    }

    indent_puts("#define yymore() (yy_more_flag = 1)");
    indent_puts("#define YY_MORE_ADJ yy_more_len");
    indent_puts("#define YY_RESTORE_YY_MORE_OFFSET");
  }

  else {
    indent_puts("#define yymore() yymore_used_but_not_detected");
    indent_puts("#define YY_MORE_ADJ 0");
    indent_puts("#define YY_RESTORE_YY_MORE_OFFSET");
  }

  if (!C_plus_plus) {
    outn("char *yytext;");
  }

  out_with_line_directive_substitution(&action_array[defs1_offset]);

  line_directive_out(scanner_c_file, 0);

  /* Copy everything up to and including the beginning of the
   * definition of the YY_INPUT macro. */
  skelout();

  if (!C_plus_plus) {
    /* Provide the body of YY_INPUT.
     *
     * TODO: It would be nice to put this into the skeleton since
     * editing these strings here is a pain. */
    if (use_read) {
      outn("  if (0) {} else { \\");
      outn("    result = read(fileno(yyin), (char*)buf, max_size); \\");
      outn("    YY_DEBUG_LOG_CALL(\"read()\", result); \\");
      outn("    if (result < 0) { \\");
      outn("      YY_FATAL_ERROR(\"input in smflex scanner failed\"); \\");
      outn("    } \\");
      outn("  }");
    }

    else {
      outn("  if (yy_current_buffer->yy_is_interactive) { \\");
      outn("    int c = '*', n; \\");
      outn("    for (n = 0; \\");
      outn("         n < max_size && (c = getc(yyin)) != EOF && c != '\\n'; \\");
      outn("         ++n) { \\");
      outn("      YY_DEBUG_LOG_CALL(\"getc()\", c); \\");
      outn("      buf[n] = (char) c; \\");
      outn("    } \\");
      outn("    /* Either we hit 'size_max' or we read a special character. */ \\");
      outn("    if (c == '\\n') { \\");
      outn("      YY_DEBUG_LOG_CALL(\"getc()\", c); \\");
      outn("      buf[n++] = (char) c; \\");
      outn("    } \\");
      outn("    if (c == EOF) { \\");
      outn("      YY_DEBUG_LOG_CALL(\"getc()\", c); \\");
      outn("      if (ferror(yyin)) { \\");
      outn("        YY_FATAL_ERROR(\"input in smflex scanner failed\"); \\");
      outn("      } \\");
      outn("    } \\");
      outn("    result = n; \\");
      outn("  } \\");
      outn("  else { \\");
      outn("    result = fread(buf, 1, max_size, yyin); \\");
      outn("    YY_DEBUG_LOG_CALL(\"fread()\", result); \\");
      outn("    if (result == 0 && ferror(yyin)) { \\");
      outn("      YY_FATAL_ERROR(\"input in smflex scanner failed\"); \\");
      outn("    } \\");
      outn("  }");
    }
  }
  else {
    /* For the C++ interface, the skeleton defines YY_INPUT. */
  }

  skelout();

  indent_puts("#define YY_RULE_SETUP \\");
  indent_up();
  if (bol_needed) {
    indent_puts("if ( yyleng > 0 ) \\");
    indent_up();
    indent_puts("yy_current_buffer->yy_at_bol = \\");
    indent_puts("    (yytext[yyleng - 1] == '\\n'); \\");
    indent_down();
  }
  indent_puts("YY_USER_ACTION");
  indent_down();

  skelout();

  /* Copy prolog to output file. */
  out(&action_array[prolog_offset]);

  line_directive_out(scanner_c_file, 0);

  skelout();

  set_indent(2);

  if (yymore_used) {
    indent_puts("yy_more_len = 0;");
    indent_puts("if ( yy_more_flag )");
    indent_lbrace();
    indent_puts("yy_more_len = yy_c_buf_p - yytext_ptr;");
    indent_puts("yy_more_flag = 0;");
    indent_rbrace();
  }

  skelout();

  gen_start_state();

  /* Note, don't use any indentation. */
  outn("yy_match:");
  gen_next_match();

  skelout();
  set_indent(2);
  gen_find_action();

  skelout();
  if (do_yylineno) {
    indent_puts("if ( yy_act != YY_END_OF_BUFFER )");
    indent_lbrace();
    indent_puts("int yyl;");
    indent_puts("for ( yyl = 0; yyl < yyleng; ++yyl )");
    indent_up();
    indent_puts("if ( yytext[yyl] == '\\n' )");
    indent_up();
    indent_puts("++yylineno;");
    indent_down();
    indent_down();
    indent_rbrace();
  }

  skelout();
  if (ddebug) {
    indent_puts("if ( yy_flex_debug )");

    indent_lbrace();
    indent_puts("if ( yy_act == 0 )");
    indent_up();
    indent_puts(C_plus_plus ?
                "cerr << \"--scanner backing up\\n\";" :
                "fprintf( stderr, \"--scanner backing up\\n\" );");
    indent_down();

    do_indent();
    out_dec("else if ( yy_act < %d )\n", num_rules);
    indent_up();

    if (C_plus_plus) {
      indent_puts
        ("cerr << \"--accepting rule at line \" << yy_rule_linenum[yy_act] <<");
      indent_puts("         \"(\\\"\" << yytext << \"\\\")\\n\";");
    }
    else {
      indent_puts
        ("fprintf( stderr, \"--accepting rule at line %d (\\\"%s\\\")\\n\",");

      indent_puts("         yy_rule_linenum[yy_act], yytext );");
    }

    indent_down();

    do_indent();
    out_dec("else if ( yy_act == %d )\n", num_rules);
    indent_up();

    if (C_plus_plus) {
      indent_puts
        ("cerr << \"--accepting default rule (\\\"\" << yytext << \"\\\")\\n\";");
    }
    else {
      indent_puts
        ("fprintf( stderr, \"--accepting default rule (\\\"%s\\\")\\n\",");
      indent_puts("         yytext );");
    }

    indent_down();

    do_indent();
    out_dec("else if ( yy_act == %d )\n", num_rules + 1);
    indent_up();

    indent_puts(C_plus_plus ?
                "cerr << \"--(end of buffer or a NUL)\\n\";" :
                "fprintf( stderr, \"--(end of buffer or a NUL)\\n\" );");

    indent_down();

    do_indent();
    outn("else");
    indent_up();

    if (C_plus_plus) {
      indent_puts
        ("cerr << \"--EOF (start condition \" << YY_START << \")\\n\";");
    }
    else {
      indent_puts
        ("fprintf( stderr, \"--EOF (start condition %d)\\n\", YY_START );");
    }

    indent_down();

    indent_rbrace();
  }

  /* Copy actions to output file. */
  skelout();
  indent_up();
  gen_bu_action();

  /* Emit all user actions associated with rules.  Up to this point,
   * they have all been accumulated in 'action_array', one after
   * another as a big string, delimited only by the 'case' statements
   * that "switch (yy_act)" uses to jump into them.
   *
   * The action string contains markers saying where we should
   * substitute #line directives pointing at the output file, which are
   * translated by 'out_with_line_directive_substitution'. */
  out_with_line_directive_substitution(&action_array[action_offset]);

  line_directive_out(scanner_c_file, 0);

  /* generate cases for any missing EOF rules */
  for (i = 1; i <= lastsc; ++i)
    if (!sceof[i]) {
      do_indent();
      out_str("case YY_STATE_EOF(%s):\n", scname[i]);
      did_eof_rule = true;
    }

  if (did_eof_rule) {
    indent_up();
    indent_puts("yyterminate();");
    indent_down();
  }


  /* Generate code for handling NUL's, if needed. */

  /* First, deal with backing up and setting up yy_cp if the scanner
   * finds that it should JAM on the NUL.
   */
  skelout();
  set_indent(6);

  if (fullspd || fulltbl)
    indent_puts("yy_cp = yy_c_buf_p;");

  else {                        /* compressed table */
    if (!reject && !interactive) {
      /* Do the guaranteed-needed backing up to figure
       * out the match.
       */
      indent_puts("yy_cp = yy_last_accepting_cpos;");
      indent_puts("yy_current_state = yy_last_accepting_state;");
    }

    else
      /* Still need to initialize yy_cp, though
       * yy_current_state was set up by
       * yy_get_previous_state().
       */
      indent_puts("yy_cp = yy_c_buf_p;");
  }


  /* Generate code for yy_get_previous_state(). */
  set_indent(1);
  skelout();

  gen_start_state();

  set_indent(2);
  skelout();
  gen_next_state(true);

  set_indent(1);
  skelout();
  gen_NUL_trans();

  skelout();
  if (do_yylineno) {            /* update yylineno inside of unput() */
    indent_puts("if ( c == '\\n' )");
    indent_up();
    indent_puts("--yylineno;");
    indent_down();
  }

  skelout();
  /* Update BOL and yylineno inside of input(). */
  if (bol_needed) {
    indent_puts("yy_current_buffer->yy_at_bol = (c == '\\n');");
    if (do_yylineno) {
      indent_puts("if ( yy_current_buffer->yy_at_bol )");
      indent_up();
      indent_puts("++yylineno;");
      indent_down();
    }
  }

  else if (do_yylineno) {
    indent_puts("if ( c == '\\n' )");
    indent_up();
    indent_puts("++yylineno;");
    indent_down();
  }

  skelout();

  /* Copy remainder of input to output. */

  line_directive_out(scanner_c_file, 1);

  if (sectnum == 3)
    (void) flexscan();          /* copy remainder of input to output */
}


/* Write 'line' and a newline to 'fp', except if it contains any
 * occurrences of "yyFlexLexer", replace the "yy" part with 'prefix'.
 *
 * If 'fp' is 'scanner_c_file', then increment 'out_linenum'. */
void emit_with_class_name_substitution(FILE * fp, char const *line)
{
  if (fp == scanner_c_file) {
    out_line_count(line);
    ++out_linenum;        /* For the final newline, below. */
  }

  /* Look for the default class name so we can change it. */
  char const *yyFlexLexer;
  while ((yyFlexLexer = strstr(line, "yyFlexLexer")) != NULL) {
    /* Replace the "yy" in 'yyFlexLexer' with 'prefix'.  By default,
     * 'prefix' is "yy", but we still use the general mechanism. */
    fprintf(fp, "%.*s%s", (int) (yyFlexLexer - line), line,  /* up to "yy" */
            prefix);                                         /* replacement */

    /* Resume after the "yy". */
    line = yyFlexLexer + 2;
  }

  /* Print the remaining line with no substitution. */
  fprintf(fp, "%s\n", line);
}


/* Write the header file containing the C++ lexer class definition. */
void emit_header_file(char const *header_file_name)
{
  FILE *header_file;

  /* Open the output file. */
  header_file = fopen(header_file_name,
                      write_native_line_endings ? "w" : "wb");
  if (header_file == NULL) {
    fprintf(stderr, _("failed to create header file \"%s\": %s\n"),
            header_file_name, strerror(errno));
    exit(2);
  }

  /* Begin with a title and description. */
  fprintf(header_file, "// %s\n", header_file_name);
  fprintf(header_file, "// Scanner class definition generated by smflex.\n");
  fprintf(header_file, "// DO NOT EDIT MANUALLY.\n\n");

  /* Copy the header contents into it. */
  {
    char const **line;
    for (line = header_skl_contents; *line != NULL; line++) {
      emit_with_class_name_substitution(header_file, *line);
    }
  }

  /* Finish up. */
  if (fclose(header_file) != 0) {
    fprintf(stderr, _("failed to close header file \"%s\": %s\n"),
            header_file_name, strerror(errno));
    exit(2);
  }
}
