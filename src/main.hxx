/*
 * Author : Mirza Saadat Baig, <me@saadat.dev>
 * Date   : 07.03.21
 * License: BSD-3-Clause-Attribution
 */
#ifndef PKGX_MAIN_HXX
#define PKGX_MAIN_HXX

// --------------------*  imports  *--------------------
#include "helpers/terminal.hxx"
#include "utils/builder.hxx"
#include "utils/extractor.hxx"
#include "utils/reader.hxx"


// --------------------*  defs  *--------------------
enum {
    NO_ERROR,
    NO_ARGUMENTS,
    MALFORMED_ARGUMENT,
    MALFORMED_DIRECTORY,
    BUILD_FAILURE,
    EXTRACTION_FAILURE,
    MALFORMED_BINARY
};


#endif //PKGX_MAIN_HXX