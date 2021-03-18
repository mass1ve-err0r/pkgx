/*
 * Author : Mirza Saadat Baig, <me@saadat.dev>
 * Date   : 09.03.21
 * License: BSD-3-Clause-Attribution
 */
#ifndef PKGX_COMPACT_ARCHIVE_HXX
#define PKGX_COMPACT_ARCHIVE_HXX

// --------------------*  imports  *--------------------
#include "../helpers/terminal.hxx"
#include <cstdint>
#include <vector>
#include <string>


// --------------------*  funcs  *--------------------
int pack(const std::vector<std::pair<std::string, uint32_t>>& files, const std::string& dest);
int unpack(char* compactarchive_buf, const std::string& dest);
char* pack_to_buffer(const std::vector<std::pair<std::string, uint32_t>>& files, size_t* buf_sz);
char** unpack_to_buffer(char* compactarchive_buf, size_t* buf_sz, std::vector<size_t>& sizes);


#endif //PKGX_COMPACT_ARCHIVE_HXX