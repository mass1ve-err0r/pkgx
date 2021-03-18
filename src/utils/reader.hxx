/*
 * Author : Mirza Saadat Baig, <me@saadat.dev>
 * Date   : 18.03.21
 * License: BSD-3-Clause-Attribution
 */
#ifndef PKGX_READER_HXX
#define PKGX_READER_HXX

// --------------------*  imports  *--------------------
#include "extractor.hxx"


// --------------------*  funcs  *--------------------
void read(const std::string& s, const std::string& p, bool is_control);


// --------------------*  defs  *--------------------
static std::vector<std::string> valid_token_control = {
        "identifier", "name", "author", "maintainer", "description",
        "version", "architecture", "section", "dependencies", "replaces",
        "conflicts"
};


#endif //PKGX_READER_HXX