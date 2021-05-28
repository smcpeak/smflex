/* version.h */
/* Define SMFLEX_VERSION. */

/* Note: The version number also appears at the top of
 * generated-scanner.skl. */

/* Version numbering scheme:
 *
 * - Major version is bumped if we make a change to the input format
 *   that severely breaks backward compatibility.  It can also be bumped
 *   for major enhancements.
 *
 * - Minor version is bumped if we make an incompatible change but one
 *   that can be accomodated by responding to the specified input
 *   version number.  It can also be bumped for significant
 *   enhancements.
 *
 * - Patch version is bumped for every release that does not introduce
 *   any input format incompatibilities.
 */
#define SMFLEX_VERSION "1.0.1"

/* Smallest value for %smflex accepted.  This will depend on the extent
 * to which backward compatibility measures are employed.  In principle,
 * it does not ever need to change, although that may be prohibitively
 * expensive in some cases. */
#define MINIMUM_SMFLEX_INPUT_VERSION 100

/* Largest value for %smflex accepted.  This should be the major
 * version times 100 plus the minor version. */
#define MAXIMUM_SMFLEX_INPUT_VERSION 101
