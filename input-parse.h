/* input-parse.h */
/* Declarations for functions defined in input-parse.y.c. */
/* See COPYING for license terms. */

/* This file is different from input-parse.y.h, which contains the token
 * codes and is output from Bison. */

#ifndef INPUT_PARSE_H
#define INPUT_PARSE_H

/* These are implemented in section 3 of input-parse.y.c.
 * See the definitions there for documentation. */
extern void build_eof_action(void);
extern void format_synerr(char msg[], char arg[]);
extern void synerr(char str[]);
extern void format_warn(char msg[], char arg[]);
extern void warn(char str[]);
extern void format_pinpoint_message(char msg[], char arg[]);
extern void pinpoint_message(char str[]);
extern void line_warning(char str[], int line);
extern void line_pinpoint(char str[], int line);
extern void yyerror(char msg[]);

/* This is the primary function that Bison emits into input-parse.y.c.
 * It parses almost the entire smflex input file, invoking the actions in
 * input-parse.y as it goes.  However, it does not consume section 3
 * of the input; the client is expected to directly invoke the input
 * scanner via 'flexscan()' to process section 3 after 'yyparse()'
 * returns. */
extern int yyparse(void);


/* Used to communicate between scanner and parser.
 *
 * smcpeak: An existing comment says:
 *
 *   The type should really be YYSTYPE, but we can't easily get our
 *   hands on it.
 *
 * This seems odd to me because input-parse.y.h declares yylval as
 * YYSTYPE.  But there is also a comment in input-parse.y about
 * YYSTYPE, and a definition of it, that leads me to beleve this
 * might not be trivial to unravel. */
extern int yylval;


#endif /* INPUT_PARSE_H */
