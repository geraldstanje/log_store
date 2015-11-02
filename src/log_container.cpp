#include "log_container.h"
#include "util.h"
#include <fstream>

log_container::log_container(std::string log_store_name, uint64_t max_log_store_size): log_appender(log_store_name, max_log_store_size),
    log_store_name_(log_store_name) {
    import_log_info();
}

log_container::~log_container() {
    export_log_info();
}

log_container_iterator log_container::begin() {
    log_container_iterator it(this);
    it.log_container_iterator_begin();
    return it;
}

log_container_iterator log_container::end() {
    log_container_iterator it(this);
    it.log_container_iterator_end();
    return it;
}

// Append a record blob to the log store.
bool log_container::append(const log_record &rec) {
    return append_record(rec);
}

// Gets the current position of the log store.
uint64_t log_container::get_position() const {
    return get_end_record_num();
}

uint64_t log_container::get_size() const {
    return get_num_of_records();
}

bool log_container::truncate(const uint64_t &position) {
    return truncate_record(position);
}

bool log_container::export_log_info() {
    if (start_record_id_ == curr_record_id_) {
        return true;
    }

    std::ofstream file;
    file.open(build_file_name(log_store_name_, "log").c_str(), std::ios::out);
    if (!file.is_open()) {
        return false;
    }

    std::vector<std::string> data = {int_to_string(start_record_id_), int_to_string(curr_record_id_)};

    for (unsigned int i = 0; i < data.size(); i++) {
        std::string line = emit_line(data[i]);
        file.write(line.c_str(), line.size());

        if (!file.good()) {
            return false;
        }
    }

    return true;
}

bool log_container::import_log_info() {
    std::ifstream file;
    file.open(build_file_name(log_store_name_, "log").c_str(), std::ios::in);
    if (!file.is_open()) {
        return false;
    }

    std::vector<std::string> data;
    std::string line;

    while (getline(file, line)) {
        data.push_back(line);
    }

    if (data.size() != 2) {
        return false;
    }

    start_record_id_ = string_to_int(data[0]);
    curr_record_id_ = string_to_int(data[1]);
    return true;
}