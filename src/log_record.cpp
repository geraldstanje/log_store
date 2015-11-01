#include "log_record.h"

log_record::log_record(std::vector<unsigned char> vec): message_(vec) {}

std::vector<unsigned char> log_record::get_message() const {
    return message_;
}

string_record::string_record(const std::string &str): log_record(std::vector<unsigned char>(str.begin(), str.end())) {}