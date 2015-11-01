#include "log_container.h"
#include "util.h"
#include <fstream>

log_container::log_container(std::string log_store_name, uint64_t max_log_store_size): log_appender(log_store_name, max_log_store_size),
    log_store_name_(log_store_name) {
    import_log_config();
}
log_container::~log_container() {
    export_log_config();
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
unsigned int log_container::get_position() {
    return get_end_record_num();
}

unsigned int log_container::get_size() {
    return get_num_of_records();
}

// Removes all data before the given position in the log store. Disk space used must remain roughly
// proportional to retained data size.
bool log_container::truncate(const uint64_t &position) {
    return truncate_record(position);
}

bool log_container::export_log_config() {
    if (start_record_id_ == curr_record_id_) {
        return true;
    }

    std::ofstream file;

    file.open(log_store_name_.c_str(), std::ios::out);
    if (!file.is_open()) {
        return false;
    }

    std::string start_record_id_str = int_to_string(start_record_id_);
    std::string curr_record_id_str = int_to_string(curr_record_id_);

    file.write(start_record_id_str.c_str(), start_record_id_str.size());
    file.write("\n", 1);
    file.write(curr_record_id_str.c_str(), curr_record_id_str.size());
    file.write("\n", 1);
    file.close();
    return true;
}

bool log_container::import_log_config() {
    std::ifstream file;

    file.open(log_store_name_.c_str(), std::ios::in);
    if (!file.is_open()) {
        return false;
    }

    std::string start_record_id_str;
    std::string curr_record_id_str;

    std::getline(file, start_record_id_str);
    std::getline(file, curr_record_id_str);
    file.close();

    start_record_id_ = string_to_int(start_record_id_str);
    curr_record_id_ = string_to_int(curr_record_id_str);
    return true;
}