/*
 * Author : Mirza Saadat Baig, <me@saadat.dev>
 * Date   : 10.03.21
 * License: BSD-3-Clause-Attribution
 */
#ifndef PKGX_EXTRACTOR_HXX
#define PKGX_EXTRACTOR_HXX

// --------------------*  imports  *--------------------
#include "../helpers/terminal.hxx"
#include "../format/base.hxx"
#include "universal.hxx"
#include "compact_archive.hxx"
#include <cstdio>
#include <filesystem>
#include <fstream>


// --------------------*  defs  *--------------------
enum EXTRACTION_TYPE {
    EXTRACT_FULL,
    EXTRACT_CONTROL,
    EXTRACT_LAYOUT,
    EXTRACT_DATA_RAW,
    EXTRACT_DATA_FULL
};
typedef enum EXTRACTION_TYPE extraction_type_t;


// --------------------*  funcs  *--------------------
bool verify_existence(const std::string&  path);
bool extract(extraction_type_t ex_type, const std::string& origin);
bool extract(extraction_type_t ex_type, const std::string& origin, const std::string& dest);


#endif //PKGX_EXTRACTOR_HXX