/*
 * Author : Mirza Saadat Baig, <me@saadat.dev>
 * Date   : 10.03.21
 * License: BSD-3-Clause-Attribution
 */
#include "extractor.hxx"


// --------------------*  private  *--------------------

bool create_directories(const std::string& path)
{
    try {
        std::error_code ec;
        std::filesystem::create_directories(path, ec);
        return ec.value() == 0;
    } catch (std::exception& exception) {
        return false;
    }
}

char* get_decompressed_file(FILE* ptr, size_t buf_sz, size_t offset, size_t* real_sz)
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
        return NULL;
    }
    *real_sz = decompress_zstd(buf, buf_sz, buf_decompressed, dec_sz);
    delete[] buf;
    return buf_decompressed;
}

bool extract_singular(FILE* ptr, pkgx_header_t* hdr, extraction_type_t et, const std::string& dst)
{
    size_t decompresed_sz;
    FILE* out = NULL;
    char* decompressed_data = NULL;
    switch (et)
    {
        case EXTRACT_CONTROL:
            LOG(INFO, "Extracting control file...");
            decompressed_data = get_decompressed_file(ptr, hdr->control_sz, 0, &decompresed_sz);
            out = fopen((dst + "/_control.json").c_str(), "wb");
            break;
        case EXTRACT_LAYOUT:
            LOG(INFO, "Extracting fs-layout file...");
            decompressed_data = get_decompressed_file(ptr, hdr->layout_sz, hdr->control_sz, &decompresed_sz);
            out = fopen((dst + "/_layout.json").c_str(), "wb");
            break;
        case EXTRACT_DATA_RAW:
            LOG(INFO, "Extracting data (raw) ...");
            decompressed_data = get_decompressed_file(ptr, hdr->data_sz, (hdr->control_sz + hdr->layout_sz), &decompresed_sz);
            out = fopen((dst + "/data.compactarchive").c_str(), "wb");
            break;
        default:
            LOG(ERROR, "Could NOT recognize extraction type!");
            return false;
    }
    fwrite(decompressed_data, decompresed_sz, 1, out);
    fclose(out);
    delete[] decompressed_data;
    return true;
}

bool extract_compactarchive(FILE* ptr, pkgx_header_t* hdr, const std::string& dst)
{
    size_t compactarchive_sz;
    char* compactarchive_raw = get_decompressed_file(ptr, hdr->data_sz, (hdr->control_sz + hdr->layout_sz), &compactarchive_sz);

    size_t layout_sz;
    char* layout_buf = get_decompressed_file(ptr, hdr->layout_sz, hdr->control_sz, &layout_sz);
    nlohmann::json layout_data = get_json_from_buffer(layout_buf);

    std::vector<size_t> elements_sz;
    char** files = unpack_to_buffer(compactarchive_raw, &compactarchive_sz, elements_sz);
    if (!files) {
        LOG(ERROR, "Failed to get files from compactarchive!");
        LOG(DEBUG, "files inside: ", elements_sz.size());
        goto ex_clean;
    }
    for (size_t i = 0; i < elements_sz.size(); ++i)
    {
        std::string filename = layout_data[i].at("name");
        FILE* out = fopen((dst + "/" + filename).c_str(), "wb");
        fwrite(files[i], elements_sz[i], 1, out);
        fclose(out);
    }

    for (size_t i = 0; i < elements_sz.size(); ++i) { delete[] files[i]; }
    delete[] files;
    elements_sz.clear();
ex_clean:
    delete[] compactarchive_raw;
    delete[] layout_buf;
    layout_data.clear();
    return true;
}


// --------------------*  public  *--------------------

bool verify_existence(const std::string& path)
{
    bool f = std::filesystem::exists(path);
    if (f) {
        return !std::filesystem::is_directory(path);
    } else { return false; }
}

bool extract(extraction_type_t ex_type, const std::string& origin)
{
    std::string target = std::filesystem::path(origin).stem();
    return extract(ex_type, origin, ("./" + target + "_pkgx"));
}

bool extract(extraction_type_t ex_type, const std::string& origin, const std::string& dest)
{
    FILE* source = fopen(origin.c_str(), "rb");
    if (!source) {
        LOG(ERROR, "Could NOT open file!");
        return false;
    }
    if (!create_directories(dest)) {
        LOG(ERROR, "Failed to create necessary directories!");
        fclose(source);
        return false;
    }
    pkgx_header_t header;
    fread(&header, sizeof(pkgx_header_t), 1, source);

    bool rv;
    if (ex_type == EXTRACT_CONTROL || ex_type == EXTRACT_LAYOUT || ex_type == EXTRACT_DATA_RAW) {
        rv = extract_singular(source, &header, ex_type, dest);
        fclose(source);
        return rv;
    } else if (ex_type == EXTRACT_DATA_FULL) {
        LOG(INFO, "Extracting data directly...");
        rv = extract_compactarchive(source, &header, dest);
        fclose(source);
        return rv;
    } else if (ex_type == EXTRACT_FULL) {
        LOG(DEBUG, "called extract full");
    } else {
        LOG(ERROR, "Extraction with invalid extraction_type !");
    }

    fclose(source);
    return false;
}