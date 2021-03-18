/*
 * Author : Mirza Saadat Baig, <me@saadat.dev>
 * Date   : 09.03.21
 * License: BSD-3-Clause-Attribution
 */
#include "compact_archive.hxx"


int pack(const std::vector<std::pair<std::string, uint32_t>>& files, const std::string& dest)
{
    /* not implemented yet (unused) */
    return 0;
}

int unpack(char* compactarchive_buf, const std::string& dest)
{
    /* not implemented yet (unused) */
    return 0;
}

char* pack_to_buffer(const std::vector<std::pair<std::string, uint32_t>>& files, size_t* buf_sz)
{
    uint32_t total_sz = 0;
    uint32_t pos = 0;
    for(auto& i: files) {
        total_sz += 4;
        total_sz += i.second;
    }
    uint32_t elements = files.size();
    *buf_sz = total_sz + 4;
    char* buffer = new(std::nothrow) char[total_sz+4];
    if (!buffer) {
        LOG(ERROR, "Failed to create buffer!");
        return NULL;
    }
    memcpy(buffer+pos, &elements, sizeof(uint32_t));
    pos += 4;
    for(auto& entry: files) {
        FILE* current_file = fopen(entry.first.c_str(), "rb");
        if (!current_file) {
            LOG(ERROR, "Could NOT open file to buffer!");
            delete[] buffer;
            return NULL;
        }
        memcpy(buffer+pos, &entry.second, sizeof(uint32_t));
        pos += 4;
        fread(buffer+pos, entry.second, 1, current_file);
        pos += entry.second;
        fclose(current_file);
    }
    return buffer;
}

char** unpack_to_buffer(char* compactarchive_buf, size_t* buf_sz, std::vector<size_t>& sizes)
{
    size_t idx = 0;
    size_t bpos = 0;
    uint32_t sz;
    uint32_t elements;
    memcpy(&elements, compactarchive_buf+bpos, sizeof(uint32_t));
    bpos += 4;
    char** buf = new char*[elements];
    while (true) {
        if (bpos >= *buf_sz) {
            break;
        }
        sz = 0;
        memcpy(&sz, compactarchive_buf+bpos, sizeof(uint32_t));
        bpos += 4;
        buf[idx] = new char[sz];
        if (!buf[idx]) {
            LOG(ERROR, "Could not create buffer for current element!");
            for(int i = 0; i < elements; i++) { delete[] buf[i]; }
            delete[] buf;
            return NULL;
        }
        memcpy(buf[idx], compactarchive_buf+bpos, sz);
        bpos += sz;
        idx++;
        sizes.emplace_back(sz);
    }
    return buf;
}