/*
 * Author : Mirza Saadat Baig, <me@saadat.dev>
 * Date   : 08.03.21
 * License: BSD-3-Clause-Attribution
 */
#ifndef PKGX_UNIVERSAL_HXX
#define PKGX_UNIVERSAL_HXX

// --------------------*  imports  *--------------------
#include "../helpers/terminal.hxx"
#include "../format/base.hxx"
#include <cstdlib>
#include <cstdio>
#include <new>
#include <zstd.h>


// --------------------*  funcs  *--------------------
char* file_to_buffer(const char* filename, long* buffer_sz);
size_t compress_zstd(void* file_buffer, size_t file_sz, void* buffer);
size_t decompress_zstd(void* file_buffer, size_t file_sz, void* buffer);


#endif //PKGX_UNIVERSAL_HXX