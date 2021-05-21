/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
