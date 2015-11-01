#include "util.h"
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>

std::string int_to_string(uint64_t number) {
    std::stringstream ss;
    ss << number;
    return ss.str();
}

uint64_t string_to_int(std::string &str) {
    std::stringstream ss(str);
    uint64_t i;
    ss >> i;
    return i;
}

std::string build_file_name(const std::string &file_name_no_ext, const std::string &file_extension, uint64_t file_number) {
    std::stringstream ss;
    ss << file_name_no_ext;

    if (file_number > 0) {
        ss << '.' << file_number;
    }

    if (!file_extension.empty()) {
        ss << '.' << file_extension;
    }

    return ss.str();
}

std::string build_tmp_file_name() {
    char buf[MAX_LEN_FILENAME];
    return std::tmpnam(buf);
}

uint64_t get_file_size(const std::string &file_name_no_ext, const uint64_t &file_id) {
    struct stat filestatus;
    stat(build_file_name(file_name_no_ext, "data", file_id).c_str(), &filestatus);
    return filestatus.st_size;
}

// unlink(2) (or unlink(1), or rm(1)) will succeed, the file will be removed from your view,
// but actually still be in place. it will be actually removed once all open file handles are closed
bool remove_file(const std::string &file_name_no_ext, const uint64_t &file_id) {
    std::string file_name = build_file_name(file_name_no_ext, "data", file_id);
    int retval = unlink(file_name.c_str());
    if (retval != 0) {
        return false;
    }
    return true;
}

bool rename_file(const std::string &old_filename, const std::string &new_filename) {
    int res = rename(old_filename.c_str(), new_filename.c_str());
    if (res != 0) {
        return false;
    }
    return true;
}