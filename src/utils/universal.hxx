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
#include <nlohmann/json.hpp>


// --------------------*  funcs  *--------------------
char* file_to_buffer(const char* filename, long* buffer_sz);
char* get_file_compressed(const char* filename, size_t* real_sz);
char* get_file_decompressed(FILE* ptr, size_t buf_sz, size_t offset, size_t* real_sz);
size_t compress_zstd(void* file_buffer, size_t file_sz, void* buffer);
void get_decompression_size(void* file_buffer, size_t file_sz, size_t* buf_estimate);
size_t decompress_zstd(void* file_buffer, size_t file_sz, void* buffer, size_t buf_sz);
nlohmann::json get_json_from_buffer(char* buffer);


#endif //PKGX_UNIVERSAL_HXX