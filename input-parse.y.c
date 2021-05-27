/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 11 "input-parse.y" /* yacc.c:339  */

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


#include "input-parse.h"               /* this module */

#include "ccl.h"                       /* cclinit, cclnegate, ccladd */
#include "ecs.h"                       /* mkeccl */
#include "flexchar.h"                  /* smflex_isascii */
#include "main.h"                      /* check_options */
#include "misc.h"                      /* add_action, copy_string, etc. */
#include "nfa.h"                       /* mkstate, mkbranch, etc. */
#include "sym.h"                       /* scinstal, sclookup */
#include "yylex.h"                     /* yylex */

#include <ctype.h>                     /* isalnum, isalpha, etc. */


int pat, scnum, eps, headcnt, trailcnt, anyccl, lastchar, i, rulelen;
int trlcontxt, xcluflg, currccl, cclsorted, varlength, variable_trail_rule;

int *scon_stk;
int scon_stk_ptr;

static int madeany = false;  /* whether we've made the '.' character class */
int previous_continued_action;  /* whether the previous rule's action was '|' */

/* Expand a POSIX character class expression. */
#define CCL_EXPR(func) \
  { \
  int c; \
  for ( c = 0; c < csize; ++c ) \
    if ( smflex_isascii(c) && func(c) ) \
      ccladd( currccl, c ); \
  }

/* While POSIX defines isblank(), it's not ANSI C. */
#define IS_BLANK(c) ((c) == ' ' || (c) == '\t')

/* On some over-ambitious machines, such as DEC Alpha's, the default
 * token type is "long" instead of "int"; this leads to problems with
 * declaring yylval in flexdef.h.  But so far, all the yacc's I've seen
 * wrap their definitions of YYSTYPE with "#ifndef YYSTYPE"'s, so the
 * following should ensure that the default token type is "int".
 */
#define YYSTYPE int


#line 139 "input-parse.y.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "input-parse.y.h".  */
#ifndef YY_YY_INPUT_PARSE_Y_H_INCLUDED
# define YY_YY_INPUT_PARSE_Y_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    CHAR = 258,
    NUMBER = 259,
    SECTEND = 260,
    SCDECL = 261,
    XSCDECL = 262,
    NAME = 263,
    PREVCCL = 264,
    EOF_OP = 265,
    OPTION_OP = 266,
    OPT_OUTFILE = 267,
    OPT_PREFIX = 268,
    OPT_YYCLASS = 269,
    OPT_YY_LEX_NAME = 270,
    OPT_YY_LEX_PARAMETERS = 271,
    CCE_ALNUM = 272,
    CCE_ALPHA = 273,
    CCE_BLANK = 274,
    CCE_CNTRL = 275,
    CCE_DIGIT = 276,
    CCE_GRAPH = 277,
    CCE_LOWER = 278,
    CCE_PRINT = 279,
    CCE_PUNCT = 280,
    CCE_SPACE = 281,
    CCE_UPPER = 282,
    CCE_XDIGIT = 283
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INPUT_PARSE_Y_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 219 "input-parse.y.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   115

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  77
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  112

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   283

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      30,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    44,     2,    38,     2,     2,     2,
      45,    46,    36,    41,    37,    49,    43,    40,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      34,    29,    35,    42,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    47,     2,    48,    33,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    31,    39,    32,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    85,    85,   116,   117,   118,   119,   123,   131,   134,
     138,   141,   144,   148,   151,   152,   157,   167,   169,   171,
     173,   177,   179,   181,   185,   197,   224,   241,   262,   267,
     270,   273,   290,   293,   295,   297,   301,   318,   369,   372,
     409,   426,   432,   437,   464,   472,   476,   483,   489,   495,
     517,   530,   547,   567,   584,   591,   594,   597,   608,   611,
     618,   644,   655,   663,   670,   671,   672,   673,   674,   675,
     676,   677,   678,   679,   680,   686,   689,   700
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CHAR", "NUMBER", "SECTEND", "SCDECL",
  "XSCDECL", "NAME", "PREVCCL", "EOF_OP", "OPTION_OP", "OPT_OUTFILE",
  "OPT_PREFIX", "OPT_YYCLASS", "OPT_YY_LEX_NAME", "OPT_YY_LEX_PARAMETERS",
  "CCE_ALNUM", "CCE_ALPHA", "CCE_BLANK", "CCE_CNTRL", "CCE_DIGIT",
  "CCE_GRAPH", "CCE_LOWER", "CCE_PRINT", "CCE_PUNCT", "CCE_SPACE",
  "CCE_UPPER", "CCE_XDIGIT", "'='", "'\\n'", "'{'", "'}'", "'^'", "'<'",
  "'>'", "'*'", "','", "'$'", "'|'", "'/'", "'+'", "'?'", "'.'", "'\"'",
  "'('", "')'", "'['", "']'", "'-'", "$accept", "goal", "sect1",
  "sect1end", "startconddecl", "namelist1", "options", "optionlist",
  "option", "sect2", "initforrule", "flexrule", "scon_stk_ptr", "scon",
  "namelist2", "sconname", "rule", "re", "re2", "series", "singleton",
  "fullccl", "ccl", "ccl_expr", "string", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,    61,
      10,   123,   125,    94,    60,    62,    42,    44,    36,   124,
      47,    43,    63,    46,    34,    40,    41,    91,    93,    45
};
# endif

#define YYPACT_NINF -55

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-55)))

#define YYTABLE_NINF -25

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      76,   -55,    34,    56,   -55,   -55,   -55,   -55,   -55,   -55,
       6,   -55,    77,     5,   -55,   -55,    29,    12,    40,    46,
      47,    59,   -55,    61,   -55,    54,   -55,    87,    90,    91,
      92,    93,    67,    30,   -55,    -1,   -55,   -55,   -55,   -55,
     -55,   -55,   -55,   -55,    49,   -55,    62,   -55,   -55,   -55,
     -55,    27,   -55,   -55,    27,    70,    74,   -55,    26,    27,
      27,    37,   -55,   -55,    97,   -55,   -55,     1,    -6,   -55,
       0,   -55,   -55,    27,   -55,   -26,    37,   102,   -55,   -55,
     -55,   -55,   -55,   -55,   -55,    32,    58,   -55,   -55,   -55,
     -55,   -55,   -55,   -55,   -55,   -55,   -55,   -55,   -55,   -55,
     -55,    27,   -55,   -22,   -55,   105,   -55,    -3,   -55,    78,
     -55,   -55
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     6,     0,     0,     1,     7,     8,     9,    15,    23,
       0,     4,    13,    32,    12,    11,     3,     0,     0,     0,
       0,     0,    14,    29,     2,    24,    10,     0,     0,     0,
       0,     0,     0,     0,    23,     0,    16,    17,    18,    19,
      20,    31,    35,    36,     0,    34,    32,    28,    57,    54,
      27,     0,    52,    77,     0,    63,     0,    26,    40,     0,
      42,    45,    53,    30,     0,    22,    25,     0,     0,    63,
       0,    21,    39,     0,    43,    37,    44,     0,    46,    47,
      48,    33,    76,    55,    56,     0,    61,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    58,
      62,    41,    38,     0,    59,     0,    51,     0,    60,     0,
      50,    49
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -55,   -55,   -55,   -55,   -55,   -55,   -55,   -55,   -55,    75,
      86,   -55,   -55,   -55,   -55,    48,    63,   -43,   -55,    42,
     -54,   -55,    44,   -55,   -55
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     3,     9,    10,    16,    11,    12,    22,    13,
      24,    56,    33,    25,    44,    45,    57,    58,    59,    60,
      61,    62,    70,   100,    67
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      47,   109,    48,    86,    82,   -24,    76,    14,    49,    50,
     106,    68,   102,    73,    15,   107,    75,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,   110,
      48,    42,    51,    73,     4,    86,    49,    26,    43,    23,
      84,    27,    52,    53,    54,    83,    55,    76,    99,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,     5,     6,     7,    72,    73,    74,     8,    77,    28,
      52,    53,    54,    78,    55,    29,    30,     1,    79,    80,
     104,    -5,    -5,    -5,    63,    34,    64,    -5,    31,    17,
      18,    19,    20,    21,    65,    36,    23,    32,    37,    38,
      39,    40,    41,    69,    71,    43,   103,   105,   108,    46,
     111,    35,    81,    85,    66,   101
};

static const yytype_uint8 yycheck[] =
{
       1,     4,     3,     3,     3,     0,    60,     1,     9,    10,
      32,    54,    38,    39,     8,    37,    59,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    32,
       3,     1,    33,    39,     0,     3,     9,     8,     8,    34,
      46,    29,    43,    44,    45,    44,    47,   101,    48,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,     5,     6,     7,    38,    39,    40,    11,    31,    29,
      43,    44,    45,    36,    47,    29,    29,     1,    41,    42,
      48,     5,     6,     7,    35,    31,    37,    11,    29,    12,
      13,    14,    15,    16,    32,     8,    34,    36,     8,     8,
       8,     8,    35,    33,    30,     8,     4,    49,     3,    34,
      32,    25,    64,    69,    51,    73
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    51,    52,     0,     5,     6,     7,    11,    53,
      54,    56,    57,    59,     1,     8,    55,    12,    13,    14,
      15,    16,    58,    34,    60,    63,     8,    29,    29,    29,
      29,    29,    36,    62,    31,    60,     8,     8,     8,     8,
       8,    35,     1,     8,    64,    65,    59,     1,     3,     9,
      10,    33,    43,    44,    45,    47,    61,    66,    67,    68,
      69,    70,    71,    35,    37,    32,    66,    74,    67,    33,
      72,    30,    38,    39,    40,    67,    70,    31,    36,    41,
      42,    65,     3,    44,    46,    72,     3,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    48,
      73,    69,    38,     4,    48,    49,    32,    37,     3,     4,
      32,    32
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    50,    51,    52,    52,    52,    52,    53,    54,    54,
      55,    55,    55,    56,    57,    57,    58,    58,    58,    58,
      58,    59,    59,    59,    60,    61,    61,    61,    61,    62,
      63,    63,    63,    64,    64,    64,    65,    66,    66,    66,
      66,    67,    67,    68,    69,    69,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    71,    71,
      72,    72,    72,    72,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    74,    74
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     3,     2,     0,     1,     1,     1,     1,
       2,     1,     1,     2,     2,     0,     3,     3,     3,     3,
       3,     5,     5,     0,     0,     2,     1,     1,     1,     0,
       4,     3,     0,     3,     1,     1,     1,     2,     3,     2,
       1,     3,     1,     2,     2,     1,     2,     2,     2,     6,
       5,     4,     1,     1,     1,     3,     3,     1,     3,     4,
       4,     2,     2,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     0
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 86 "input-parse.y" /* yacc.c:1646  */
    { /* add default rule */
                          int def_rule;

                          pat = cclinit();
                          cclnegate(pat);

                          def_rule = mkstate(-pat);

                          /* Remember the number of the default rule so we
                           * don't generate "can't match" warnings for it.
                           */
                          default_rule = num_rules;

                          finish_rule(def_rule, false, 0, 0);

                          for (i = 1; i <= lastsc; ++i)
                            scset[i] = mkbranch(scset[i], def_rule);

                          add_action(yy_output_file_line_directive);
                          if (option_suppress_default_rule) {
                            add_action("  YY_ERROR_NO_RULE_MATCHES();\n"
                                       "  return 0;\n");
                          }
                          else {
                            add_action("  YY_ECHO;\n"
                                       "  YY_BREAK\n");
                          }
                        }
#line 1408 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 6:
#line 120 "input-parse.y" /* yacc.c:1646  */
    { synerr("unknown error processing section 1"); }
#line 1414 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 7:
#line 124 "input-parse.y" /* yacc.c:1646  */
    {
                          check_options();
                          scon_stk = allocate_integer_array(lastsc + 1);
                          scon_stk_ptr = 0;
                        }
#line 1424 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 8:
#line 132 "input-parse.y" /* yacc.c:1646  */
    { xcluflg = false; }
#line 1430 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 9:
#line 135 "input-parse.y" /* yacc.c:1646  */
    { xcluflg = true; }
#line 1436 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 10:
#line 139 "input-parse.y" /* yacc.c:1646  */
    { scinstal(nmstr, xcluflg); }
#line 1442 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 11:
#line 142 "input-parse.y" /* yacc.c:1646  */
    { scinstal(nmstr, xcluflg); }
#line 1448 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 12:
#line 145 "input-parse.y" /* yacc.c:1646  */
    { synerr("bad start condition list"); }
#line 1454 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 16:
#line 158 "input-parse.y" /* yacc.c:1646  */
    {
                          /* Only record the specified file name if we
                           * do not already have one.  This way, the -o
                           * command line option takes precedence. */
                          if (!did_outfilename) {
                            outfilename = copy_string(nmstr);
                            did_outfilename = 1;
                          }
                        }
#line 1468 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 17:
#line 168 "input-parse.y" /* yacc.c:1646  */
    { set_prefix(copy_string(nmstr)); }
#line 1474 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 18:
#line 170 "input-parse.y" /* yacc.c:1646  */
    { option_yyclass = copy_string(nmstr); }
#line 1480 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 19:
#line 172 "input-parse.y" /* yacc.c:1646  */
    { option_yy_lex_name = copy_string(nmstr); }
#line 1486 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 20:
#line 174 "input-parse.y" /* yacc.c:1646  */
    { option_yy_lex_parameters = copy_string(nmstr); }
#line 1492 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 21:
#line 178 "input-parse.y" /* yacc.c:1646  */
    { scon_stk_ptr = (yyvsp[-3]); }
#line 1498 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 22:
#line 180 "input-parse.y" /* yacc.c:1646  */
    { scon_stk_ptr = (yyvsp[-3]); }
#line 1504 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 24:
#line 185 "input-parse.y" /* yacc.c:1646  */
    {
                          /* Initialize for a parse of one rule. */
                          trlcontxt = variable_trail_rule = varlength = false;
                          trailcnt = headcnt = rulelen = 0;
                          current_state_type = STATE_NORMAL;
                          previous_continued_action = continued_action;
                          in_rule = true;

                          new_rule();
                        }
#line 1519 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 25:
#line 198 "input-parse.y" /* yacc.c:1646  */
    {
                          pat = (yyvsp[0]);
                          finish_rule(pat, variable_trail_rule, headcnt, trailcnt);

                          if (scon_stk_ptr > 0) {
                            for (i = 1; i <= scon_stk_ptr; ++i)
                              scbol[scon_stk[i]] = mkbranch(scbol[scon_stk[i]], pat);
                          }

                          else {
                            /* Add to all non-exclusive start conditions,
                             * including the default (0) start condition.
                             */
                            for (i = 1; i <= lastsc; ++i)
                              if (!scxclu[i])
                                scbol[i] = mkbranch(scbol[i], pat);
                          }

                          if (!bol_needed) {
                            bol_needed = true;

                            if (performance_report > 1)
                              pinpoint_message("'^' operator results in sub-optimal performance");
                          }
                        }
#line 1549 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 26:
#line 225 "input-parse.y" /* yacc.c:1646  */
    {
                          pat = (yyvsp[0]);
                          finish_rule(pat, variable_trail_rule, headcnt, trailcnt);

                          if (scon_stk_ptr > 0) {
                            for (i = 1; i <= scon_stk_ptr; ++i)
                              scset[scon_stk[i]] = mkbranch(scset[scon_stk[i]], pat);
                          }

                          else {
                            for (i = 1; i <= lastsc; ++i)
                              if (!scxclu[i])
                                scset[i] = mkbranch(scset[i], pat);
                          }
                        }
#line 1569 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 27:
#line 242 "input-parse.y" /* yacc.c:1646  */
    {
                          if (scon_stk_ptr > 0)
                            build_eof_action();

                          else {
                            /* This EOF applies to all start conditions
                             * which don't already have EOF actions.
                             */
                            for (i = 1; i <= lastsc; ++i)
                              if (!sceof[i])
                                scon_stk[++scon_stk_ptr] = i;

                            if (scon_stk_ptr == 0)
                              warn("all start conditions already have <<EOF>> rules");

                            else
                              build_eof_action();
                          }
                        }
#line 1593 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 28:
#line 263 "input-parse.y" /* yacc.c:1646  */
    { synerr("unrecognized rule"); }
#line 1599 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 29:
#line 267 "input-parse.y" /* yacc.c:1646  */
    { (yyval) = scon_stk_ptr; }
#line 1605 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 30:
#line 271 "input-parse.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-2]); }
#line 1611 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 31:
#line 274 "input-parse.y" /* yacc.c:1646  */
    {
                          (yyval) = scon_stk_ptr;

                          for (i = 1; i <= lastsc; ++i) {
                            int j;

                            for (j = 1; j <= scon_stk_ptr; ++j)
                              if (scon_stk[j] == i)
                                break;

                            if (j > scon_stk_ptr)
                              scon_stk[++scon_stk_ptr] = i;
                          }
                        }
#line 1630 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 32:
#line 290 "input-parse.y" /* yacc.c:1646  */
    { (yyval) = scon_stk_ptr; }
#line 1636 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 35:
#line 298 "input-parse.y" /* yacc.c:1646  */
    { synerr("bad start condition list"); }
#line 1642 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 36:
#line 302 "input-parse.y" /* yacc.c:1646  */
    {
                          if ((scnum = sclookup(nmstr)) == 0)
                            format_pinpoint_message("undeclared start condition %s", nmstr);
                          else {
                            for (i = 1; i <= scon_stk_ptr; ++i)
                              if (scon_stk[i] == scnum) {
                                format_warn("<%s> specified twice", scname[scnum]);
                                break;
                              }

                            if (i > scon_stk_ptr)
                              scon_stk[++scon_stk_ptr] = scnum;
                          }
                        }
#line 1661 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 37:
#line 319 "input-parse.y" /* yacc.c:1646  */
    {
                          if (transchar[lastst[(yyvsp[0])]] != SYM_EPSILON)
                            /* Provide final transition \now/ so it
                             * will be marked as a trailing context
                             * state.
                             */
                            (yyvsp[0]) = link_machines((yyvsp[0]), mkstate(SYM_EPSILON));

                          mark_beginning_as_normal((yyvsp[0]));
                          current_state_type = STATE_NORMAL;

                          if (previous_continued_action) {
                            /* We need to treat this as variable trailing
                             * context so that the backup does not happen
                             * in the action but before the action switch
                             * statement.  If the backup happens in the
                             * action, then the rules "falling into" this
                             * one's action will *also* do the backup,
                             * erroneously.
                             */
                            if (!varlength || headcnt != 0)
                              warn("trailing context made variable due to preceding '|' action");

                            /* Mark as variable. */
                            varlength = true;
                            headcnt = 0;
                          }

                          /* variable trailing context rule */
                          if (varlength && headcnt == 0) {
                            /* Mark the first part of the rule as the
                             * accepting "head" part of a trailing
                             * context rule.
                             *
                             * By the way, we didn't do this at the
                             * beginning of this production because back
                             * then current_state_type was set up for a
                             * trail rule, and add_accept() can create
                             * a new state ...
                             */
                            add_accept((yyvsp[-1]), num_rules | YY_TRAILING_HEAD_MASK);
                            variable_trail_rule = true;
                          }

                          else
                            trailcnt = rulelen;

                          (yyval) = link_machines((yyvsp[-1]), (yyvsp[0]));
                        }
#line 1715 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 38:
#line 370 "input-parse.y" /* yacc.c:1646  */
    { synerr("trailing context used twice"); }
#line 1721 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 39:
#line 373 "input-parse.y" /* yacc.c:1646  */
    {
                          headcnt = 0;
                          trailcnt = 1;
                          rulelen = 1;
                          varlength = false;

                          current_state_type = STATE_TRAILING_CONTEXT;

                          if (trlcontxt) {
                            synerr("trailing context used twice");
                            (yyval) = mkstate(SYM_EPSILON);
                          }

                          else if (previous_continued_action) {
                            /* See the comment in the rule for "re2 re"
                             * above.
                             */
                            warn("trailing context made variable due to preceding '|' action");

                            varlength = true;
                          }

                          if (varlength) {
                            /* Again, see the comment in the rule for
                             * "re2 re" above.
                             */
                            add_accept((yyvsp[-1]), num_rules | YY_TRAILING_HEAD_MASK);
                            variable_trail_rule = true;
                          }

                          trlcontxt = true;

                          eps = mkstate(SYM_EPSILON);
                          (yyval) = link_machines((yyvsp[-1]), link_machines(eps, mkstate('\n')));
                        }
#line 1761 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 40:
#line 410 "input-parse.y" /* yacc.c:1646  */
    {
                          (yyval) = (yyvsp[0]);

                          if (trlcontxt) {
                            if (varlength && headcnt == 0)
                              /* Both head and trail are
                               * variable-length.
                               */
                              variable_trail_rule = true;
                            else
                              trailcnt = rulelen;
                          }
                        }
#line 1779 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 41:
#line 427 "input-parse.y" /* yacc.c:1646  */
    {
                          varlength = true;
                          (yyval) = mkor((yyvsp[-2]), (yyvsp[0]));
                        }
#line 1788 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 42:
#line 433 "input-parse.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1794 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 43:
#line 438 "input-parse.y" /* yacc.c:1646  */
    {
                          /* This rule is written separately so the
                           * reduction will occur before the trailing
                           * series is parsed.
                           */

                          if (trlcontxt)
                            synerr("trailing context used twice");
                          else
                            trlcontxt = true;

                          if (varlength)
                            /* We hope the trailing context is
                             * fixed-length.
                             */
                            varlength = false;
                          else
                            headcnt = rulelen;

                          rulelen = 0;

                          current_state_type = STATE_TRAILING_CONTEXT;
                          (yyval) = (yyvsp[-1]);
                        }
#line 1823 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 44:
#line 465 "input-parse.y" /* yacc.c:1646  */
    {
                          /* This is where concatenation of adjacent patterns
                           * gets done.
                           */
                          (yyval) = link_machines((yyvsp[-1]), (yyvsp[0]));
                        }
#line 1834 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 45:
#line 473 "input-parse.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[0]); }
#line 1840 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 46:
#line 477 "input-parse.y" /* yacc.c:1646  */
    {
                          varlength = true;

                          (yyval) = mkclos((yyvsp[-1]));
                        }
#line 1850 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 47:
#line 484 "input-parse.y" /* yacc.c:1646  */
    {
                          varlength = true;
                          (yyval) = mkposcl((yyvsp[-1]));
                        }
#line 1859 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 48:
#line 490 "input-parse.y" /* yacc.c:1646  */
    {
                          varlength = true;
                          (yyval) = mkopt((yyvsp[-1]));
                        }
#line 1868 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 49:
#line 496 "input-parse.y" /* yacc.c:1646  */
    {
                          varlength = true;

                          if ((yyvsp[-3]) > (yyvsp[-1]) || (yyvsp[-3]) < 0) {
                            synerr("bad iteration values");
                            (yyval) = (yyvsp[-5]);
                          }
                          else {
                            if ((yyvsp[-3]) == 0) {
                              if ((yyvsp[-1]) <= 0) {
                                synerr("bad iteration values");
                                (yyval) = (yyvsp[-5]);
                              }
                              else
                                (yyval) = mkopt(mkrep((yyvsp[-5]), 1, (yyvsp[-1])));
                            }
                            else
                              (yyval) = mkrep((yyvsp[-5]), (yyvsp[-3]), (yyvsp[-1]));
                          }
                        }
#line 1893 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 50:
#line 518 "input-parse.y" /* yacc.c:1646  */
    {
                          varlength = true;

                          if ((yyvsp[-2]) <= 0) {
                            synerr("iteration value must be positive");
                            (yyval) = (yyvsp[-4]);
                          }

                          else
                            (yyval) = mkrep((yyvsp[-4]), (yyvsp[-2]), INFINITY);
                        }
#line 1909 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 51:
#line 531 "input-parse.y" /* yacc.c:1646  */
    {
                          /* The singleton could be something like "(foo)",
                           * in which case we have no idea what its length
                           * is, so we punt here.
                           */
                          varlength = true;

                          if ((yyvsp[-1]) <= 0) {
                            synerr("iteration value must be positive");
                            (yyval) = (yyvsp[-3]);
                          }

                          else
                            (yyval) = link_machines((yyvsp[-3]), copysingl((yyvsp[-3]), (yyvsp[-1]) - 1));
                        }
#line 1929 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 52:
#line 548 "input-parse.y" /* yacc.c:1646  */
    {
                          if (!madeany) {
                            /* Create the '.' character class. */
                            anyccl = cclinit();
                            ccladd(anyccl, '\n');
                            cclnegate(anyccl);

                            if (useecs)
                              mkeccl(ccltbl + cclmap[anyccl],
                                     ccllen[anyccl], nextecm, ecgroup, csize, csize);

                            madeany = true;
                          }

                          ++rulelen;

                          (yyval) = mkstate(-anyccl);
                        }
#line 1952 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 53:
#line 568 "input-parse.y" /* yacc.c:1646  */
    {
                          if (!cclsorted)
                            /* Sort characters for fast searching.  We
                             * use a shell sort since this list could
                             * be large.
                             */
                            cshell(ccltbl + cclmap[(yyvsp[0])], ccllen[(yyvsp[0])], true);

                          if (useecs)
                            mkeccl(ccltbl + cclmap[(yyvsp[0])], ccllen[(yyvsp[0])], nextecm, ecgroup, csize, csize);

                          ++rulelen;

                          (yyval) = mkstate(-(yyvsp[0]));
                        }
#line 1972 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 54:
#line 585 "input-parse.y" /* yacc.c:1646  */
    {
                          ++ rulelen;

                          (yyval) = mkstate(-(yyvsp[0]));
                        }
#line 1982 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 55:
#line 592 "input-parse.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 1988 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 56:
#line 595 "input-parse.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 1994 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 57:
#line 598 "input-parse.y" /* yacc.c:1646  */
    {
                          ++rulelen;

                          if (caseins && (yyvsp[0]) >= 'A' && (yyvsp[0]) <= 'Z')
                            (yyvsp[0]) = clower((yyvsp[0]));

                          (yyval) = mkstate((yyvsp[0]));
                        }
#line 2007 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 58:
#line 609 "input-parse.y" /* yacc.c:1646  */
    { (yyval) = (yyvsp[-1]); }
#line 2013 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 59:
#line 612 "input-parse.y" /* yacc.c:1646  */
    {
                          cclnegate((yyvsp[-1]));
                          (yyval) = (yyvsp[-1]);
                        }
#line 2022 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 60:
#line 619 "input-parse.y" /* yacc.c:1646  */
    {
                          if (caseins) {
                            if ((yyvsp[-2]) >= 'A' && (yyvsp[-2]) <= 'Z')
                              (yyvsp[-2]) = clower((yyvsp[-2]));
                            if ((yyvsp[0]) >= 'A' && (yyvsp[0]) <= 'Z')
                              (yyvsp[0]) = clower((yyvsp[0]));
                          }

                          if ((yyvsp[-2]) > (yyvsp[0]))
                            synerr("negative range in character class");

                          else {
                            for (i = (yyvsp[-2]); i <= (yyvsp[0]); ++i)
                              ccladd((yyvsp[-3]), i);

                            /* Keep track if this ccl is staying in
                             * alphabetical order.
                             */
                            cclsorted = cclsorted && ((yyvsp[-2]) > lastchar);
                            lastchar = (yyvsp[0]);
                          }

                          (yyval) = (yyvsp[-3]);
                        }
#line 2051 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 61:
#line 645 "input-parse.y" /* yacc.c:1646  */
    {
                          if (caseins && (yyvsp[0]) >= 'A' && (yyvsp[0]) <= 'Z')
                            (yyvsp[0]) = clower((yyvsp[0]));

                          ccladd((yyvsp[-1]), (yyvsp[0]));
                          cclsorted = cclsorted && ((yyvsp[0]) > lastchar);
                          lastchar = (yyvsp[0]);
                          (yyval) = (yyvsp[-1]);
                        }
#line 2065 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 62:
#line 656 "input-parse.y" /* yacc.c:1646  */
    {
                          /* Too hard to properly maintain cclsorted. */
                          cclsorted = false;
                          (yyval) = (yyvsp[-1]);
                        }
#line 2075 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 63:
#line 663 "input-parse.y" /* yacc.c:1646  */
    {
                          cclsorted = true;
                          lastchar = 0;
                          currccl = (yyval) = cclinit();
                        }
#line 2085 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 64:
#line 670 "input-parse.y" /* yacc.c:1646  */
    { CCL_EXPR(isalnum) }
#line 2091 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 65:
#line 671 "input-parse.y" /* yacc.c:1646  */
    { CCL_EXPR(isalpha) }
#line 2097 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 66:
#line 672 "input-parse.y" /* yacc.c:1646  */
    { CCL_EXPR(IS_BLANK) }
#line 2103 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 67:
#line 673 "input-parse.y" /* yacc.c:1646  */
    { CCL_EXPR(iscntrl) }
#line 2109 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 68:
#line 674 "input-parse.y" /* yacc.c:1646  */
    { CCL_EXPR(isdigit) }
#line 2115 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 69:
#line 675 "input-parse.y" /* yacc.c:1646  */
    { CCL_EXPR(isgraph) }
#line 2121 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 70:
#line 676 "input-parse.y" /* yacc.c:1646  */
    { CCL_EXPR(islower) }
#line 2127 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 71:
#line 677 "input-parse.y" /* yacc.c:1646  */
    { CCL_EXPR(isprint) }
#line 2133 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 72:
#line 678 "input-parse.y" /* yacc.c:1646  */
    { CCL_EXPR(ispunct) }
#line 2139 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 73:
#line 679 "input-parse.y" /* yacc.c:1646  */
    { CCL_EXPR(isspace) }
#line 2145 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 74:
#line 680 "input-parse.y" /* yacc.c:1646  */
    {
                                  if (caseins)
                                    CCL_EXPR(islower)
                                  else
                                    CCL_EXPR(isupper)
                                }
#line 2156 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 75:
#line 686 "input-parse.y" /* yacc.c:1646  */
    { CCL_EXPR(isxdigit) }
#line 2162 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 76:
#line 690 "input-parse.y" /* yacc.c:1646  */
    {
                          if (caseins && (yyvsp[0]) >= 'A' && (yyvsp[0]) <= 'Z')
                            (yyvsp[0]) = clower((yyvsp[0]));

                          ++rulelen;

                          (yyval) = link_machines((yyvsp[-1]), mkstate((yyvsp[0])));
                        }
#line 2175 "input-parse.y.c" /* yacc.c:1646  */
    break;

  case 77:
#line 700 "input-parse.y" /* yacc.c:1646  */
    { (yyval) = mkstate(SYM_EPSILON); }
#line 2181 "input-parse.y.c" /* yacc.c:1646  */
    break;


#line 2185 "input-parse.y.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 703 "input-parse.y" /* yacc.c:1906  */



/* build_eof_action - build the "<<EOF>>" action for the active start
 *                    conditions
 */
void build_eof_action()
{
  int i;
  char action_text[MAXLINE];

  for (i = 1; i <= scon_stk_ptr; ++i) {
    if (sceof[scon_stk[i]])
      format_pinpoint_message("multiple <<EOF>> rules for start condition %s",
                              scname[scon_stk[i]]);

    else {
      sceof[scon_stk[i]] = true;
      sprintf(action_text, "case YY_STATE_EOF(%s):\n", scname[scon_stk[i]]);
      add_action(action_text);
    }
  }

  line_directive_out_src();

  /* This isn't a normal rule after all - don't count it as
   * such, so we don't have any holes in the rule numbering
   * (which make generating "rule can never match" warnings
   * more difficult.
   */
  --num_rules;
  ++num_eof_rules;
}


/* format_synerr - write out formatted syntax error */
void format_synerr(char const *msg, char const *arg)
{
  char errmsg[MAXLINE];

  (void) sprintf(errmsg, msg, arg);
  synerr(errmsg);
}


/* synerr - report a syntax error */
void synerr(char str[])
{
  syntaxerror = true;
  pinpoint_message(str);
}


/* Report a syntax error where 'fmt' has one integer placeholder. */
void synerr_i(char const *fmt, int n)
{
  char errmsg[MAXLINE];
  sprintf(errmsg, fmt, n);
  synerr(errmsg);
}


/* Report a syntax error where 'fmt' has two integer placeholders. */
void synerr_ii(char const *fmt, int n1, int n2)
{
  char errmsg[MAXLINE];
  sprintf(errmsg, fmt, n1, n2);
  synerr(errmsg);
}


/* format_warn - write out formatted warning */
void format_warn(char msg[], char arg[])
{
  char warn_msg[MAXLINE];

  (void) sprintf(warn_msg, msg, arg);
  warn(warn_msg);
}


/* warn - report a warning, unless -w was given */
void warn(char str[])
{
  line_warning(str, linenum);
}

/* format_pinpoint_message - write out a message formatted with one string,
 *                           pinpointing its location
 */
void format_pinpoint_message(char const *msg, char const *arg)
{
  /* Extra 30 for "undeclared start condition %s" at caller, in order to
   * silence a GCC warning.  The extra space is not really needed--or
   * rather, if huge names are used in the scanner definition, this is
   * far from the only buffer that will overflow. */
  char errmsg[MAXLINE+30];

  (void) sprintf(errmsg, msg, arg);
  pinpoint_message(errmsg);
}


/* pinpoint_message - write out a message, pinpointing its location */
void pinpoint_message(char str[])
{
  line_pinpoint(str, linenum);
}


/* line_warning - report a warning at a given line, unless -w was given */
void line_warning(char str[], int line)
{
  /* Extra 10 for "warning, %s". */
  char warning[MAXLINE+10];

  if (!nowarn) {
    sprintf(warning, "warning, %s", str);
    line_pinpoint(warning, line);
  }
}


/* line_pinpoint - write out a message, pinpointing it at the given line */
void line_pinpoint(char str[], int line)
{
  fprintf(stderr, "%s:%d: %s\n", infilename, line, str);
}


/* yyerror - eat up an error message from the parser;
 *           currently, messages are ignore
 */
void yyerror(char msg[])
{
}
