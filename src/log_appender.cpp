#include "log_appender.h"
#include "util.h"
#include <fstream>
#include <iostream>

log_appender::log_appender(std::string log_store_name): log_store_name_(log_store_name),
    start_record_id_(0),
    end_record_id_(0) {}

log_appender::~log_appender() {}

uint64_t log_appender::get_start_record_num() const {
    return start_record_id_;
}

uint64_t log_appender::get_end_record_num() const {
    return end_record_id_;
}

uint64_t log_appender::get_num_of_records() const {
    return end_record_id_ - start_record_id_;
}

bool log_appender::write_record_tmp_file(log_record &rec, const std::string &tmp_file_name) {
    std::ofstream file;
    std::vector<char> data = rec.get_message();
    uint64_t total_bytes_to_write = data.size();

    if (total_bytes_to_write == 0 ||
            total_bytes_to_write > max_record_size ||
            total_bytes_to_write > get_available_free_space()) {
        return false;
    }

    file.open(tmp_file_name.c_str(), std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    file.write(data.data(), total_bytes_to_write);
    if (!file.good()) {
        return false;
    }

    return true;
}

bool log_appender::append_record(log_record &rec) {
    std::string tmp_file_name = build_tmp_file_name();

    if (!write_record_tmp_file(rec, tmp_file_name)) {
        return false;
    }

    // get the lock
    std::lock_guard<std::mutex> lock(mutex_);

    if (!rename_file(tmp_file_name, build_file_name(log_store_name_, "data", end_record_id_))) {
        return false;
    }

    end_record_id_++;
    return true;
}

bool log_appender::read_record(const uint64_t &record_id, log_record &record) {
    std::ifstream file;
    uint64_t total_size = 0;

    {
        // get the lock
        std::lock_guard<std::mutex> lock(mutex_);

        if (record_id < start_record_id_ || record_id >= end_record_id_) {
            return false;
        }

        total_size = get_file_size(log_store_name_, record_id);

        file.open(build_file_name(log_store_name_, "data", record_id).c_str(), std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            return false;
        }
    }

    file.read(record.get_message_data(total_size), total_size);
    return true;
}

bool log_appender::truncate_record(const uint64_t &position) {
    uint64_t start_record_id_tmp = 0;

    {
        // get the lock
        std::lock_guard<std::mutex> lock(mutex_);

        start_record_id_tmp = start_record_id_;

        if (position < start_record_id_ || position >= end_record_id_) {
            return false;
        }

        start_record_id_ = position + 1;
    }

    uint64_t record_id = start_record_id_tmp;

    while (record_id <= position) {
        if (!remove_file(log_store_name_, record_id)) {
            return false;
        }
        record_id++;
    }

    return true;
}