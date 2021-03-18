/*
 * Author : Mirza Saadat Baig, <me@saadat.dev>
 * Date   : 07.03.21
 * License: BSD-3-Clause-Attribution
 */
#include "main.hxx"


int main(int argc, char** argv)
{
    if (argc < 2) {
        LOG(ERROR, "No arguments supplied! Aborting...");
        exit(NO_ARGUMENTS);
    }
    if (strcmp("-h", argv[1]) == 0) {
        print_help();
        exit(NO_ERROR);
    } else if (strcmp("-b", argv[1]) == 0) {
        bool build_success;
        bool directory_ok;
        bool build_external = false;
        std::string path_global;
        switch (argc) {
            case 2:
                LOG(INFO, "Building locally...");
                directory_ok = verify_structure(".");
                path_global = ".";
                break;
            case 4:
                build_external = true;
            case 3:
                LOG(INFO, "Building from: ", argv[2]);
                directory_ok = verify_structure(argv[2]);
                path_global = argv[2];
                break;
            default:
                goto not_recognized;
        }
        if (!directory_ok) {
            LOG(ERROR, "Directory structure mismatch! Aborting...");
            exit(MALFORMED_DIRECTORY);
        }
        LOG(SUCCESS, "Verified directory structure");
        build_success = (build_external) ? build(path_global, argv[3]) : build(path_global);
        if (!build_success) {
            LOG(ERROR, "Error during build stages! Aborting...");
            exit(BUILD_FAILURE);
        }
        LOG(SUCCESS, "Successfully built package!");
        exit(NO_ERROR);
    } else if (std::string_view(argv[1]).substr(1, 1).compare("x") == 0) {
        bool file_exists;
        bool extract_external = false;
        bool extract_success;
        switch (argc) {
            case 4:
                extract_external = true;
            case 3:
                file_exists = verify_existence(argv[2]);
                break;
            default:
                goto not_recognized;
        }
        if (!file_exists) {
            LOG(ERROR, "File does NOT exist! Aborting...");
            exit(MALFORMED_BINARY);
        }
        if (strcmp("-x", argv[1]) == 0) {
            extract_success = (extract_external) ? extract(EXTRACT_FULL, argv[2], argv[3]) : extract(EXTRACT_FULL, argv[2]);
        } else if (strcmp("-xc", argv[1]) == 0) {
            extract_success = (extract_external) ? extract(EXTRACT_CONTROL, argv[2], argv[3]) : extract(EXTRACT_CONTROL, argv[2]);
        } else if (strcmp("-xl", argv[1]) == 0) {
            extract_success = (extract_external) ? extract(EXTRACT_LAYOUT, argv[2], argv[3]) : extract(EXTRACT_LAYOUT, argv[2]);
        } else if (strcmp("-xdr", argv[1]) == 0) {
            extract_success = (extract_external) ? extract(EXTRACT_DATA_RAW, argv[2], argv[3]) : extract(EXTRACT_DATA_RAW, argv[2]);
        } else if (strcmp("-xdf", argv[1]) == 0) {
            extract_success = (extract_external) ? extract(EXTRACT_DATA_FULL, argv[2], argv[3]) : extract(EXTRACT_DATA_FULL, argv[2]);
        } else {
            goto not_recognized;
        }
        if (!extract_success) {
            LOG(ERROR, "Error during extract stages! Aborting...");
            exit(EXTRACTION_FAILURE);
        }
        LOG(SUCCESS, "Successfully extracted from package!");
        exit(NO_ERROR);
    } else if (strcmp("-rc", argv[1]) == 0 && argc == 4) {
        read(argv[2], argv[3], true);
        return NO_ERROR;
    } else if (strcmp("-rl", argv[1]) == 0 && argc == 4) {
        read(argv[2], argv[3], false);
        return NO_ERROR;
    }

not_recognized:
    LOG(ERROR, "Argument not recognized! Aborting...");
    return MALFORMED_ARGUMENT;
}