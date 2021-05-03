/* flexchar.h */
/* Definition of 'Char'. */

#ifndef FLEXCHAR_H
#define FLEXCHAR_H

/* Always be prepared to generate an 8-bit scanner. */
#define CSIZE 256
#define Char unsigned char

/* This is a POSIX function, not standard C. */
#ifndef smflex_isascii
#  define smflex_isascii(c) ((c) <= 0177)
#endif

#endif /* FLEXCHAR_H */
