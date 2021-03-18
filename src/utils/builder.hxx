/*
 * Author : Mirza Saadat Baig, <me@saadat.dev>
 * Date   : 07.03.21
 * License: BSD-3-Clause-Attribution
 */
#ifndef PKGX_BUILDER_HXX
#define PKGX_BUILDER_HXX

// --------------------*  imports  *--------------------
#include "../helpers/terminal.hxx"
#include "../format/base.hxx"
#include "universal.hxx"
#include "compact_archive.hxx"
#include <cstdio>
#include <filesystem>
#include <string>
#include <vector>


// --------------------*  funcs  *--------------------
bool build(const std::string& origin);
bool build(const std::string& origin, const std::string& dest);
bool verify_structure(const std::string& path);


#endif //PKGX_BUILDER_HXX