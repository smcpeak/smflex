/* readfile.c */
/* Simply read the contents of a file, in order to provide a baseline
 * against which to compare the speed of a scanner. *
 *
 * See ../COPYING for license terms. */

/* This file is meant to be compiled either as C or C++ code.  It
 * provides the option to test the C++ library when compiled as C++. */

#include <stddef.h>                    /* size_t, ssize_t */
#include <stdio.h>                     /* FILE, fread, fileno */
#include <stdlib.h>                    /* malloc, free, exit */
#include <string.h>                    /* strcmp */

#ifdef HAS_POSIX
#include <unistd.h>                    /* read */
#endif

#ifdef __cplusplus
#include <fstream>                     /* ifstream */
using namespace std;
#endif


/* Characters read and processed.  This might not be the number of
 * bytes in the file if text mode processing is enabled and the
 * library is translating line endings. */
static unsigned int numCharactersRead = 0;

/* Counts of each character value encountered. */
static unsigned int characterCount[256] = {0};

/* Sum of every character value. */
static unsigned int characterSum = 0;


static void processData(unsigned char const *buffer, size_t n)
{
  /* Do some very light processing of the input, something
   * approximating the minimum amount of useful computation that a
   * program might perform on a file's contents. */
  unsigned char const *p = buffer;
  unsigned char const *end = buffer + n;
  for (; p < end; p++) {
    int c = *p;
    numCharactersRead++;
    characterCount[c]++;
    characterSum += c;
  }
}


static void readFile(char const *method, unsigned char *buffer,
                     int bufsize, char const *inputFname, int binmode)
{
  if (0==strcmp(method, "fread")) {
    FILE *fp = stdin;

    if (inputFname) {
      fp = fopen(inputFname, binmode? "rb" : "r");
      if (!fp) {
        printf("failed to open file: \"%s\"\n", inputFname);
        exit(2);
      }
    }

    do {
      size_t n = fread(buffer, 1, bufsize, fp);
      if (n < bufsize && ferror(fp)) {
        printf("error while reading \"%s\"\n", inputFname);
        exit(2);
      }

      processData(buffer, n);
    } while (!feof(fp));

    if (inputFname) {
      fclose(fp);
    }
  }

  else if (0==strcmp(method, "read")) {
#   ifdef HAS_POSIX
      FILE *fp = stdin;
      int fd;
      ssize_t n;

      if (inputFname) {
        /* Ordinarily we would use POSIX 'open', but that is not
         * how smflex does things; instead, it assumes that 'fopen'
         * will be used, so calls 'fileno'. */
        fp = fopen(inputFname, binmode? "rb" : "r");
        if (!fp) {
          printf("failed to open file: \"%s\"\n", inputFname);
          exit(2);
        }
      }

      fd = fileno(fp);
      while ((n = read(fd, buffer, bufsize)) != 0) {
        if (n < 0) {
          printf("error while reading \"%s\"\n", inputFname);
          exit(2);
        }

        processData(buffer, n);
      }

      if (inputFname) {
        fclose(fp);
      }

#   else
      printf("HAS_POSIX not defined, cannot use \"read\" method\n");
      exit(2);
#   endif
  }

  else {
    printf("unknown read method: \"%s\n", method);
  }
}


int main(int argc, char **argv)
{
  /* Index of next argument to consume. */
  int nextArgIndex = 0;

  /* True if there is another argument. */
  #define HAS_ARG (nextArgIndex < argc)

  /* Get the next argument and advance the index. */
  #define NEXT_ARG (argv[nextArgIndex++])

  char const *programName = NEXT_ARG;

  if (!HAS_ARG) {
    printf("usage: %s (fread|read|iostream) [bufsize] [filename] [iters] [binmode]\n\n",
           programName);
    printf("The first argument specifies how to read the input:\n"
           "  fread:    Use C 'fread'.\n"
           "  read:     Use POSIX 'read'.  Must compile with HAS_UNISTD_H.\n"
           "  iostream: Use C++ iostreams.  Must compile with a C++ compiler.\n"
           "\n"
           "If [bufsize] is omitted, use 8192 bytes.  Accepts hex with 0x prefix.\n"
           "\n"
           "If [filename] is omitted, read stdin.\n"
           "\n"
           "if [iters] is omitted, read the input one time.\n"
           "\n"
           "The [binmode] argument, if provided, specifies:\n"
           "  t:        Read in text mode (default).\n"
           "  b:        Read in binary mode.\n"
    );
    return 2;
  }

  char const *method = NEXT_ARG;

  int bufsize = 8192;
  if (HAS_ARG) {
    bufsize = strtol(NEXT_ARG, NULL /*endp*/, 0 /*radix*/);
    printf("bufsize: %d (0x%X)\n", bufsize, bufsize);
  }

  /* We do the buffer allocation once, outside the loop, to ensure
   * best case allocator traffic and locality. */
  unsigned char *buffer = malloc(bufsize);

  int iters = 1;

  if (HAS_ARG) {
    /* Read a named file. */
    char const *inputFname = NEXT_ARG;
    int binmode = 0;
    int i;

    printf("input file name: %s\n", inputFname);

    if (HAS_ARG) {
      /* Specify number of iterations. */
      iters = atoi(NEXT_ARG);
      printf("iters: %d\n", iters);
    }

    if (HAS_ARG) {
      char const *binmodeName = NEXT_ARG;
      if (0==strcmp(binmodeName, "t")) {
        binmode = 0;
      }
      else if (0==strcmp(binmodeName, "b")) {
        binmode = 1;
      }
      else {
        printf("unknown binmode name: \"%s\n", binmodeName);
        exit(2);
      }
      printf("binmode: %d\n", binmode);
    }

    for (i=0; i < iters; i++) {
      readFile(method, buffer, bufsize, inputFname, binmode);
    }
  }
  else {
    /* Read stdin. */
    readFile(method, buffer, bufsize, NULL /*inputFname*/, 0 /*binmode*/);
  }

  /* Print some stats. */
  printf("num characters read: %u\n", numCharactersRead);
  printf("sum of all character values: %u\n", characterSum);
  if (iters > 1) {
    /* Abbreviated character stats. */
    printf("number of 'e' in input: %u\n", characterCount['e']);
  }
  else {
    /* Full character stats. */
    int i;
    printf("character counts:\n"
           "  SPC=%u TAB=%u NL=%u CR=%u",
           characterCount[' '],
           characterCount['\t'],
           characterCount['\n'],
           characterCount['\r']);
    for (i=33; i<=126; i++) {
      if (i % 8 == 1) {
        printf("\n ");
      }
      printf(" %c=%6u", (char)i, characterCount[i]);
    }
    printf("\n");
  }

  free(buffer);
  return 0;
}


/* EOF */
