#ifndef LOG_APPENDER_H
#define LOG_APPENDER_H

#include "log_record.h"
#include <cstdint>
#include <mutex>

const uint64_t max_record_size = 32 * 1024 * 1024; // in MB

class log_appender {
  protected:
    std::string log_store_name_;
    uint64_t start_record_id_;
    uint64_t end_record_id_;
    std::mutex mutex_;

  private:
    bool write_record_tmp_file(log_record &rec, const std::string &tmp_file_name);

  public:
    log_appender(std::string log_store_name);
    ~log_appender();
    uint64_t get_start_record_num() const;
    uint64_t get_end_record_num() const;
    uint64_t get_num_of_records() const;
    bool append_record(log_record &rec);
    bool read_record(const uint64_t &record_id, log_record &record);
    bool truncate_record(const uint64_t &position);
};

#endif