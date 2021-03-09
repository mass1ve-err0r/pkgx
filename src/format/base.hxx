/*
 * Author : Mirza Saadat Baig, <me@saadat.dev>
 * Date   : 07.03.21
 * License: BSD-3-Clause-Attribution
 */
#ifndef PKGX_BASE_HXX
#define PKGX_BASE_HXX

// --------------------*  imports  *--------------------
#include "structures.hxx"


// --------------------*  defs  *--------------------
inline uint32_t PKGX_MAGIC = 0xdeadc0de;
inline uint32_t PKGX_CIGAM = 0xdec0adde;
inline uint32_t PKGX_MAX_SIZE = 1073741824;


// --------------------*  macros  *--------------------
#define IS_LITTLE_ENDIAN(x) ((x) == PKGX_MAGIC || (x) == PKGX_CIGAM)
#define OS_INDEPENDENT_SWAP32(x) ((((x) & 0xff000000) >> 24) | (((x) & 0xff0000) >> 8) | (((x) & 0xff00) << 8) | (((x) & 0xff) << 24))
#define SWAP32(x, magic) (IS_LITTLE_ENDIAN(magic)? OS_INDEPENDENT_SWAP32(x): (x))


#endif //PKGX_BASE_HXX