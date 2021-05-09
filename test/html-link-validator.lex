/* html-link-validator.lex */
/* Validate local and intra-document links in HTML. */
/* Written by Scott McPeak and placed into the public domain. */


%{
#include <assert.h>                    /* assert */
#include <errno.h>                     /* errno */
#include <stdio.h>                     /* printf */
#include <stdlib.h>                    /* malloc, free, getenv */
#include <string.h>                    /* strncpy, strerror */


enum {
  /* Maximum number of anchors within the document. */
  MAX_ANCHORS = 1000,

  /* Maximum number of intra-document links. */
  MAX_LINKS = 1000,
};

/* Number of anchors found.*/
int numAnchors = 0;

/* Entries [0, numAnchors-1] contain malloc'd pointers to the
 * discovered anchors, in discovery order. */
char *anchors[MAX_ANCHORS];

/* Record an intra-document link in the input. */
typedef struct Link {
  int lineNumber;            /* Line where link appears. */
  char *anchor;              /* malloc'd anchor name. */
} Link;

/* Number of intra-document links found. */
int numLinks = 0;

/* Entries [0,numLinks-1] contain the discovered links. */
Link links[MAX_LINKS];

/* Number of errors encountered. */
int numErrors = 0;


/* Forwards. */
static char *my_strndup(char const *src, int len);

%}

%option yylineno

WS          [ \t\n]

%%

  /* 'id' attributes define anchors. */
id=\"[^"]+\"       {
                     assert(numAnchors < MAX_ANCHORS);
                     anchors[numAnchors++] = my_strndup(yytext+4, yyleng-5);
                   }

  /* Intra-document link. */
href=\"#[^"]+\"    {
                     assert(numLinks < MAX_LINKS);
                     links[numLinks].lineNumber = yylineno;
                     links[numLinks].anchor = my_strndup(yytext+7, yyleng-8);
                     numLinks++;
                   }

  /* Relative link to a local file.  See if we can open it. */
href=\"[^:"]+\"    {
                     char *fname = my_strndup(yytext+6, yyleng-7);
                     FILE *fp = fopen(fname, "r");
                     if (!fp) {
                       printf("Link on line %d to \"%s\": cannot open file.\n",
                              yylineno, fname);
                       numErrors++;
                     }
                     else {
                       fclose(fp);
                     }
                     free(fname);
                   }

.|\n               /* discard */

%%


/* Return a NUL-terminated, malloc'd string containing the characters
 * in the range [src,src+len-1]. */
static char *my_strndup(char const *src, int len)
{
  char *ret = (char*)malloc(len+1);
  assert(ret);
  strncpy(ret, src, len);
  ret[len] = '\0';
  return ret;
}


int main(int argc, char **argv)
{
  int i, j;

  yylex();

  for (i=0; i < numLinks; i++) {
    /* Inefficient linear search. */
    for (j=0; j < numAnchors; j++) {
      if (0==strcmp(links[i].anchor, anchors[j])) {
        break;    /* found */
      }
    }
    if (j == numAnchors) {
      printf("Link on line %d goes to non-existent anchor \"%s\".\n",
             links[i].lineNumber, links[i].anchor);
      numErrors++;
    }
  }

  if (numErrors > 0) {
    printf("Errors: %d\n", numErrors);

    /* By default I do not exit non-zero in this case because that
     * would cause the unit test to fail.  I set this envvar in
     * the place I want non-zero exits. */
    if (getenv("FAIL_IF_ERRORS")) {
      return 2;
    }
  }

  return 0;
}


/* EOF */
