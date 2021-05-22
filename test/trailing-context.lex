/* trailing-context.lex */
/* Example with trailing context, mainly for performance testing. */
/* See ../COPYING for license terms. */

%smflex 100
%{

#include "perftest.h"                  /* runPerftest */

/* Counts of various occurrences. */
static int numPrefixBeforeSuffix=0;
static int numPrefixWithoutSuffix=0;
static int numSuffix=0;
static int numOther=0;

%}

%%

#.*"\n"               /* Comment, ignore. */
pref+ix/suf+ix        numPrefixBeforeSuffix++;
pref+ix               numPrefixWithoutSuffix++;
suf+ix                numSuffix++;
[^ \t\n]+             numOther++;
[ \t\n]+              /* Nothing. */

%%

int main(int argc, char **argv)
{
  runPerftest(argc, argv);

  #define PVAL(val) printf(#val ": %d\n", val)
  PVAL(numPrefixBeforeSuffix);
  PVAL(numPrefixWithoutSuffix);
  PVAL(numSuffix);
  PVAL(numOther);

  return 0;
}

/* EOF */
