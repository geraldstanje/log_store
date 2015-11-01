#ifndef LOG_APPENDER_H
#define LOG_APPENDER_H

#include "log_record.h"
#include <cstdint>
#include <mutex>

class log_appender {
  public:
    std::string log_store_name_;
    uint64_t start_record_id_;
    uint64_t curr_record_id_;
    std::mutex mutex_;

  public:
    log_appender(std::string log_store_name, uint64_t max_size);
    ~log_appender();

    bool check_internal_data();
    uint64_t get_start_record_num();
    uint64_t get_end_record_num();
    uint64_t get_num_of_records();
    // todo: add error handling
    bool append_record(const log_record &rec);
    bool read_record(const uint64_t &record_id, std::string &record);
    bool truncate_record(const uint64_t &position);
};

#endif