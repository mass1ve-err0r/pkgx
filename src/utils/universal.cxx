/*
 * Author : Mirza Saadat Baig, <me@saadat.dev>
 * Date   : 08.03.21
 * License: BSD-3-Clause-Attribution
 */
#include "universal.hxx"


char* file_to_buffer(const char* filename, long* buffer_sz)
{
    FILE* src = fopen(filename, "rb");
    if (!src) {
        *buffer_sz = -1;
        return NULL;
    }
    fseeko(src, 0, SEEK_END);
    *buffer_sz = ftello(src);
    fseeko(src,  0, SEEK_SET);
    char* buffer = new(std::nothrow) char[*buffer_sz];
    if (!buffer) {
        *buffer_sz = -2;    // buffer alloc failed
        return NULL;
    }
    fread(buffer, *buffer_sz, 1, src);
    fclose(src);
    return buffer;
}

size_t compress_zstd(void* file_buffer, size_t file_sz, void* buffer)
{
    size_t buffer_sz = ZSTD_compressBound(file_sz);
    size_t outfile_sz = ZSTD_compress(buffer, buffer_sz, file_buffer, file_sz, ZSTD_maxCLevel());
    if (ZSTD_isError(outfile_sz)) {
        LOG(ERROR, "An error occurred during compression with zstd!");
        return 0;
    }
    return outfile_sz;
}

size_t decompress_zstd(void* file_buffer, size_t file_sz, void* buffer)
{
    uint64_t buffer_sz = ZSTD_getFrameContentSize(file_buffer, file_sz);
    size_t output_sz = ZSTD_decompress(buffer, buffer_sz, file_buffer, file_sz);
    if (ZSTD_isError(output_sz)) {
        LOG(ERROR, "An error occurred during decompression with zstd!");
        return 0;
    }
    return output_sz;
}