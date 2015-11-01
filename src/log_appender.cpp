#include "log_appender.h"
#include "util.h"
#include <fstream>

log_appender::log_appender(uint64_t max_size): start_record_id_(0), curr_record_id_(0) {}

log_appender::~log_appender() {}

bool log_appender::check_internal_data() {
    if (start_record_id_ != curr_record_id_) {
        return false;
    }
    return true;
}

uint64_t log_appender::get_start_record_num() {
    return start_record_id_;
}

uint64_t log_appender::get_end_record_num() {
    return curr_record_id_;
}

uint64_t log_appender::get_num_of_records() {
    return curr_record_id_ - start_record_id_;
}

bool log_appender::append_record(const log_record &rec) {
    std::ofstream file;
    bool retval = true;

    std::vector<unsigned char> data = rec.get_message();
    uint64_t total_bytes_to_write = data.size();
    std::string tmp_file = build_tmp_file_name();

    file.open(tmp_file.c_str(), std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    file.write(reinterpret_cast<const char*>(&data[0]), total_bytes_to_write);
    file.close();

    // get the lock
    std::lock_guard<std::mutex> lock(mutex_);

    rename_file(tmp_file, build_file_name("F", "txt", curr_record_id_));
    curr_record_id_++;
    return retval;
}

bool log_appender::read_record(const uint64_t &record_id, std::string &record) {
    {
        // get the lock
        std::lock_guard<std::mutex> lock(mutex_);

        if (curr_record_id_ < start_record_id_ || record_id >= curr_record_id_) {
            return false;
        }
    }

    uint64_t total_size = get_file_size(record_id);
    record.resize(total_size, 0);

    std::ifstream file;
    file.open(build_file_name("F", "txt", record_id).c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    file.read(&record[0], total_size);
    file.close();
    return true;
}

bool log_appender::truncate_record(const uint64_t &position) {
    uint64_t start_record_id_tmp = start_record_id_;

    {
        // get the lock
        std::lock_guard<std::mutex> lock(mutex_);

        if (position < start_record_id_ || position >= curr_record_id_) {
            return false;
        }

        start_record_id_ = position + 1;
    }

    uint64_t record_id = start_record_id_tmp;

    while (record_id <= position) {
        if (!remove_file(record_id)) {
            return false;
        }
        record_id++;
    }

    return true;
}