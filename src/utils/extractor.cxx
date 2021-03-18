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

bool extract_singular(FILE* ptr, pkgx_header_t* hdr, extraction_type_t et, const std::string& dst)
{
    bool rv = true;
    size_t decompresed_sz;
    FILE* out = NULL;
    char* decompressed_data = NULL;
    switch (et)
    {
        case EXTRACT_CONTROL:
            LOG(INFO, "Extracting control...");
            decompressed_data = get_file_decompressed(ptr, hdr->control_sz, 0, &decompresed_sz);
            out = fopen((dst + "/_control.json").c_str(), "wb");
            break;
        case EXTRACT_LAYOUT:
            LOG(INFO, "Extracting fs-layout...");
            decompressed_data = get_file_decompressed(ptr, hdr->layout_sz, hdr->control_sz, &decompresed_sz);
            out = fopen((dst + "/_layout.json").c_str(), "wb");
            break;
        case EXTRACT_DATA_RAW:
            LOG(INFO, "Extracting data (raw) ...");
            decompressed_data = get_file_decompressed(ptr, hdr->data_sz, (hdr->control_sz + hdr->layout_sz), &decompresed_sz);
            out = fopen((dst + "/data.compactarchive").c_str(), "wb");
            break;
        default:
            LOG(ERROR, "Could NOT recognize extraction type!");
            return false;
    }
    if (decompressed_data) {
        fwrite(decompressed_data, decompresed_sz, 1, out);
    } else {
        LOG(ERROR, "Failed to get raw data! Aborting...");
        rv = false;
    }
    fclose(out);
    delete[] decompressed_data;
    return rv;
}

bool extract_compactarchive(FILE* ptr, pkgx_header_t* hdr, const std::string& dst, bool from_fs = true)
{
    bool rv = true;
    char* compactarchive_raw;
    char* layout_buf = NULL;
    nlohmann::json layout_data;
    char** files = NULL;
    LOG(INFO, "Extracting compactarchive...");

    size_t compactarchive_sz;
    compactarchive_raw = get_file_decompressed(ptr, hdr->data_sz, (hdr->control_sz + hdr->layout_sz), &compactarchive_sz);

    if (from_fs) {
        size_t layout_sz;
        layout_buf = get_file_decompressed(ptr, hdr->layout_sz, hdr->control_sz, &layout_sz);
        layout_data = get_json_from_buffer(layout_buf);
    } else {
        std::ifstream l(dst + "/_layout.json");
        l >> layout_data;
        l.close();
    }

    std::vector<size_t> elements_sz;
    files = unpack_to_buffer(compactarchive_raw, &compactarchive_sz, elements_sz);
    if (!files) {
        LOG(ERROR, "Failed to get files from compactarchive!");
        rv = false;
        goto cleanup;
    }
    for (size_t i = 0; i < elements_sz.size(); ++i)
    {
        std::string filename = layout_data[i].at("name");
        filename.insert(0, (dst + "/"));
        FILE* out = fopen((filename).c_str(), "wb");
        if (!out) {
            LOG(ERROR, "Could not create actual file for current element!");
            rv = false;
            goto cleanup;
        }
        fwrite(files[i], elements_sz[i], 1, out);
        fclose(out);
    }

cleanup:
    for (size_t i = 0; i < elements_sz.size(); ++i) { delete[] files[i]; }
    delete[] files;
    elements_sz.clear();
    delete[] compactarchive_raw;
    delete[] layout_buf;
    layout_data.clear();
    return rv;
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
        rv = extract_singular(source, &header, EXTRACT_CONTROL, dest);
        if (rv) {
            bool rv2 = extract_singular(source, &header, EXTRACT_LAYOUT, dest);
            if (rv2) {
                bool rv3 = extract_compactarchive(source, &header, dest, false);
                if (rv3) {
                    fclose(source);
                    return true;
                } else { LOG(ERROR, "Failed on stage 3! Aborting..."); }
            } else { LOG(ERROR, "Failed on stage 2! Aborting..."); }
        } else { LOG(ERROR, "Failed on stage 1! Aborting..."); }
    } else {
        LOG(ERROR, "Extraction with invalid extraction_type !");
    }

    fclose(source);
    return false;
}