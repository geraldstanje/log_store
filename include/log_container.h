#ifndef LOG_CONTAINER_H
#define LOG_CONTAINER_H

#include "log_appender.h"
#include "log_container_iterator.h"
#include <cstdint>
#include <string>

class log_container: public log_appender {
  private:
    std::string log_store_name_;

  private:
    bool export_log_info();
    bool import_log_info();

  public:
    log_container(std::string log_store_name, uint64_t max_log_store_size);
    ~log_container();
    log_container_iterator begin();
    log_container_iterator end();
    // Append a record blob to the log store.
    bool append(const log_record &rec);
    // Removes all data before the given position in the log store. Disk space used must remain roughly
    // proportional to retained data size.
    bool truncate(const uint64_t &position);
    // Gets the current position of the log store.
    uint64_t get_position() const;
    uint64_t get_size() const;
};

#endif