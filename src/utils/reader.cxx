/*
 * Author : Mirza Saadat Baig, <me@saadat.dev>
 * Date   : 18.03.21
 * License: BSD-3-Clause-Attribution
 */
#include "reader.hxx"


bool user_input_loop()
{
    bool rv = true;
    std::string input;
    while (input != "y" or input != "n")
    {
        std::cout << "Do you want to list all instead? (y/n)" << "\n";
        std::cin >> input;
        if (input == "y") {
            rv = true;
            break;
        } else if (input == "n") {
            rv = false;
            break;
        }
    }
    std::cin.clear();
    return rv;
}

void read(const std::string& src, const std::string& prop, bool is_control)
{
    bool exists = verify_existence(src);
    if (exists) {
        FILE* source = fopen(src.c_str(), "rb");
        pkgx_header_t header;
        fread(&header, sizeof(pkgx_header_t), 1, source);

        size_t data_sz;
        char* data = NULL;
        if (is_control) {
            data = get_file_decompressed(source, header.control_sz, 0, &data_sz);
        } else {
            data = get_file_decompressed(source, header.layout_sz, header.control_sz, &data_sz);
        }
        if(!data) {
            LOG(ERROR, "Could not create buffer! Aborting...");
            fclose(source);
            return;
        }
        nlohmann::json json = get_json_from_buffer(data);

        if (is_control) {
            LOG(INFO2, "Reading ", prop);
            int elems;
            if (std::find(valid_token_control.begin(), valid_token_control.end(), prop) != valid_token_control.end()) {
                if (prop == "dependencies" || prop == "conflicts") {
                    elems = json[prop].size();
                    LOG(INFO2, "Elements: ", elems);
                    for(auto& el: json[prop].items()) {
                        std::cout << "→ " << el.value()["name"] << "\n";
                        if (el.value().contains("min")) {
                            std::cout << COLOR_YELLOW << " ↳ Min version: " << el.value()["min"] << COLOR_RESET << "\n";
                        }
                        if (el.value().contains("max")) {
                            std::cout << COLOR_YELLOW << " ↳ Max version: " << el.value()["max"] << COLOR_RESET << "\n";
                        }
                    }
                } else if (prop == "replaces") {
                    elems = json[prop].size();
                    LOG(INFO2, "Elements: ", elems);
                    for(auto& el: json[prop].items())
                    {
                        std::cout << "→ " << el.value() << "\n";
                    }
                } else {
                    std::cout << json[prop] << "\n";
                }
            } else { LOG(ERROR, "Key not valid! Aborting..."); }
        } else {
            try {
                long idx = std::stoi(prop);
                int elems = json.size();
                if (idx >= 0 && idx < elems) {
                    LOG(INFO2, "Getting element ", (idx+1), "/", elems);
                    auto element = json[idx];
                    std::cout << "→ " << element["name"] << "\n";
                    std::cout << COLOR_YELLOW << " ↳ Permissions: " <<  element["perm"] << COLOR_RESET << "\n";
                    std::cout << COLOR_YELLOW << " ↳ Install Location: " <<  element["dest"] << COLOR_RESET << "\n";
                    if (element.contains("link")) {
                        std::cout << COLOR_YELLOW << " ↳ Symlink: " << element["link"] << COLOR_RESET << "\n";
                    }
                } else {
                    LOG(ERROR, "Invalid index!");
                    bool list_all = user_input_loop();
                    if (list_all) {
                        LOG(INFO2, "Listing all (", elems ,") elements...");
                        for (int i = 0; i < elems; ++i)
                        {
                            auto element = json[i];
                            std::cout << "→ " << element["name"] << "\n";
                            std::cout << COLOR_YELLOW << " ↳ Permissions: " <<  element["perm"] << COLOR_RESET << "\n";
                            std::cout << COLOR_YELLOW << " ↳ Install Location: " <<  element["dest"] << COLOR_RESET << "\n";
                            if (element.contains("link")) {
                                std::cout << COLOR_YELLOW << " ↳ Symlink: " << element["link"] << COLOR_RESET << "\n";
                            }
                        }
                    } else {
                        LOG(ERROR, "Aborting...");
                    }
                }
            } catch(std::exception& e) { LOG(ERROR, "Could not parse number! Aborting..."); }
        }

        delete[] data;
        fclose(source);
    }
}