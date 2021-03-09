/*
 * Author : Mirza Saadat Baig, <me@saadat.dev>
 * Date   : 07.03.21
 * License: BSD-3-Clause-Attribution
 */
#ifndef PKGX_COMMON_HXX
#define PKGX_COMMON_HXX

// --------------------*  imports  *--------------------


// --------------------*  defs  *--------------------

inline int PKGX_ERRNO = 0;
inline const char* PKGX_VERSION = "1.0";

enum {
    NO_ERROR,
    NO_ARGUMENTS,
    MALFORMED_ARGUMENT,
    MALFORMED_DIRECTORY,
    BUILD_FAILURE,
    EXTRACTION_FAILURE,
    MALFORMED_BINARY
};




#endif //PKGX_COMMON_HXX