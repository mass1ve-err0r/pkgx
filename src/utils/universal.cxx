/*
 * Author : Mirza Saadat Baig, <me@saadat.dev>
 * Date   : 08.03.21
 * License: BSD-3-Clause-Attribution
 */
#include "universal.hxx"


// --------------------*  fs  *--------------------

char* file_to_buffer(const char* filename, long* buffer_sz)
{
    FILE* src = fopen(filename, "rb");
    if (!src) {
        *buffer_sz = 0;
        return NULL;
    }
    fseeko(src, 0, SEEK_END);
    *buffer_sz = ftello(src);
    fseeko(src,  0, SEEK_SET);
    char* buffer = new(std::nothrow) char[*buffer_sz];
    if (!buffer) {
        *buffer_sz = 0;
        return NULL;
    }
    fread(buffer, *buffer_sz, 1, src);
    fclose(src);
    return buffer;
}


char* get_file_compressed(const char* filename, size_t* real_sz)
{
    long buf_sz;
    char* buf_in = file_to_buffer(filename, &buf_sz);
    if (!buf_in) {
        LOG(ERROR, "Could NOT load file into buffer!");
        return NULL;
    }
    char* buf_out = new(std::nothrow) char[buf_sz];
    if (!buf_out) {
        LOG(ERROR, "Could NOT create output buffer!");
        delete[] buf_in;
        return NULL;
    }
    *real_sz = compress_zstd(buf_in, buf_sz, buf_out);
    if (*real_sz == 0) {
        delete[] buf_in;
        delete[] buf_out;
        return NULL;
    }
    delete[] buf_in;
    return buf_out;
}

char* get_file_decompressed(FILE* ptr, size_t buf_sz, size_t offset, size_t* real_sz)
{
    size_t loc_old = ftello(ptr);
    char* buf = new(std::nothrow) char[buf_sz];
    if (!buf) {
        LOG(ERROR, "Failed to create buffer!");
        return NULL;
    }
    fseeko(ptr, offset, SEEK_CUR);
    fread(buf, buf_sz, 1, ptr);
    fseeko(ptr, loc_old, SEEK_SET);
    size_t dec_sz;
    get_decompression_size(buf, buf_sz, &dec_sz);
    char* buf_decompressed = new(std::nothrow) char[dec_sz];
    if (!buf_decompressed) {
        LOG(ERROR, "Failed to create decompression buffer!");
        delete[] buf;
        return NULL;
    }
    *real_sz = decompress_zstd(buf, buf_sz, buf_decompressed, dec_sz);
    delete[] buf;
    return buf_decompressed;
}


// --------------------*  compression  *--------------------

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

void get_decompression_size(void* file_buffer, size_t file_sz, size_t* buf_estimate)
{
    *buf_estimate = ZSTD_getFrameContentSize(file_buffer, file_sz);
}

size_t decompress_zstd(void* file_buffer, size_t file_sz, void* buffer, size_t buffer_sz)
{
    size_t output_sz = ZSTD_decompress(buffer, buffer_sz, file_buffer, file_sz);
    if (ZSTD_isError(output_sz)) {
        LOG(ERROR, "An error occurred during decompression with zstd!");
        return 0;
    }
    return output_sz;
}


// --------------------*  JSON  *--------------------

nlohmann::json get_json_from_buffer(char* buf)
{
    nlohmann::json json_obj = nlohmann::json::parse(buf, nullptr, false, false);
    if (json_obj.is_discarded()) {
        LOG(ERROR, "Layout data is corrupted!");
        return NULL;
    }
    return json_obj;
}