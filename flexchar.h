/* flexchar.h */
/* Definition of 'Char'. */

#ifndef FLEXCHAR_H
#define FLEXCHAR_H

/* Always be prepared to generate an 8-bit scanner. */
#define CSIZE 256
#define Char unsigned char

/* 'isascii' is a POSIX function, not standard C, so we define our own. */
#ifndef smflex_isascii
#  define smflex_isascii(c) (((unsigned char)(c)) <= 0177)
#endif

#endif /* FLEXCHAR_H */
