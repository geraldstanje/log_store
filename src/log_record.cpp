#include "log_record.h"

log_record::log_record() {}
log_record::log_record(std::vector<char> &&vec): message_(vec) {}

std::vector<char> log_record::get_message() {
    return std::move(message_);
}

void log_record::resize(uint64_t size) {
    message_.resize(size, 0);
}

char* log_record::get_message_ptr() {
    return &message_[0];
}

string_record::string_record(const std::string &str): log_record(std::vector<char>(str.begin(), str.end())) {}