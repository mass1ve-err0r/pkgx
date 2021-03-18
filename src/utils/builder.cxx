/*
 * Author : Mirza Saadat Baig, <me@saadat.dev>
 * Date   : 07.03.21
 * License: BSD-3-Clause-Attribution
 */
#include "builder.hxx"


// --------------------*  private  *--------------------

std::vector<std::pair<std::string, uint32_t>> collect_files(std::string_view path)
{
    std::vector<std::pair<std::string, uint32_t>> list;
    for(const auto& entry: std::filesystem::recursive_directory_iterator(path)) {
        if (entry.path().filename() == ".DS_Store" ||
            entry.path().filename() == "_control.json" ||
            entry.path().filename() == "_layout.json" ||
            entry.is_directory()) {
            continue;
        }
        list.emplace_back(entry.path(), std::filesystem::file_size(entry.path()));
    }
    return list;
}


// --------------------*  public  *--------------------

bool verify_structure(const std::string& path)
{
    bool component_1 = std::filesystem::exists(path + "/_control.json");
    bool component_2 = std::filesystem::exists(path + "/_layout.json");
    return (component_1 && component_2);
}

bool build(const std::string& origin)
{
    std::string target = std::filesystem::current_path().filename();
    return build(origin, ("./" + target + ".pkgx"));
}

bool build(const std::string& origin, const std::string& dest)
{
    bool rv = true;
    const std::string suffix = ".pkgx";
    char* control_zstd_buffer = NULL;
    char* layout_zstd_buffer = NULL;
    char* data_buf = NULL;
    char* data_zstd_buffer = NULL;
    pkgx_header_t* header = NULL;
    FILE* pkgx_file = NULL;

    if (!std::equal(suffix.rbegin(), suffix.rend(), dest.rbegin())) {
        LOG(ERROR, "Destination is NOT a valid filename!");
        return false;
    }
    LOG(INFO, "Listing files...");
    std::vector<std::pair<std::string, uint32_t>> files = collect_files(origin);
    if (files.empty()) {
        LOG(ERROR, "Directory appears to be empty (no additional files found)!");
        rv = false;
        goto cleanup;
    }
    size_t c_sz, l_sz, d_sz, dr_sz;
    LOG(INFO, "Compressing...");
    control_zstd_buffer = get_file_compressed((origin + "/_control.json").c_str(), &c_sz);
    if (!control_zstd_buffer) {
        LOG(ERROR, "Failed to create stage 1 buffer!");
        rv = false;
        goto cleanup;
    }
    layout_zstd_buffer = get_file_compressed((origin + "/_layout.json").c_str(), &l_sz);
    if (!layout_zstd_buffer) {
        LOG(ERROR, "Failed to create stage 2 buffer!");
        rv = false;
        goto cleanup;
    }
    data_buf = pack_to_buffer(files, &d_sz);
    if (!data_buf) {
        LOG(ERROR, "Failed to create stage 3 buffer!");
        rv = false;
        goto cleanup;
    }
    data_zstd_buffer = new(std::nothrow) char[d_sz];
    if (!data_zstd_buffer) {
        LOG(ERROR, "Failed to create compressed data buffer!");
        rv = false;
        goto cleanup;
    }
    dr_sz = compress_zstd(data_buf, d_sz, data_zstd_buffer);
    if (dr_sz == 0) {
        rv = false;
        goto cleanup;
    }

    header = new pkgx_header_t();
    header->magic = PKGX_MAGIC;
    header->control_sz = c_sz;
    header->layout_sz = l_sz;
    header->data_sz = dr_sz;

    LOG(INFO, "Building package...");
    pkgx_file = fopen(dest.c_str(), "wb");
    if (!pkgx_file) {
        LOG(ERROR, "Failed to create output file!");
        rv = false;
        goto cleanup;
    }
    fwrite(header, sizeof(pkgx_header_t), 1, pkgx_file);
    fwrite(control_zstd_buffer, c_sz, 1, pkgx_file);
    fwrite(layout_zstd_buffer, l_sz, 1, pkgx_file);
    fwrite(data_zstd_buffer, dr_sz, 1, pkgx_file);
    fclose(pkgx_file);

cleanup:
    delete[] data_buf;
    delete[] control_zstd_buffer;
    delete[] layout_zstd_buffer;
    delete[] data_zstd_buffer;
    delete header;
    files.clear();

    return rv;
}