#ifndef UTIL_H
#define UTIL_H

#include <string>

std::string int_to_string(uint64_t number);
uint64_t string_to_int(std::string &str);
std::string build_file_name(const std::string &file_name_no_ext, const std::string &file_extension, uint64_t file_number = 0);
std::string build_tmp_file_name();
uint64_t get_file_size(const std::string &file_name_no_ext, const uint64_t &file_id);
bool remove_file(const std::string &file_name_no_ext, const uint64_t &file_id);
bool rename_file(const std::string &old_filename, const std::string &new_filename);

#endif