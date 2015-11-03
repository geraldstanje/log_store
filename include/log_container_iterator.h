#ifndef LOG_CONTAINER_ITERATOR_H
#define LOG_CONTAINER_ITERATOR_H

#include "log_appender.h"
#include <cstdint>
#include <vector>

class log_container_iterator {
  private:
    log_appender *ptr_;
    uint64_t curr_record_id_;

  public:
    log_container_iterator(log_appender *ptr);
    log_container_iterator(const log_container_iterator &other);
    bool operator== (const log_container_iterator &it);
    bool operator!= (const log_container_iterator &it);
    uint64_t get_curr_record_id() const;
    log_record operator* ();
    log_record get_curr_record();
    void operator++(int);
    void operator++();
    log_container_iterator operator+(uint64_t offset) const;
    void log_container_iterator_begin();
    void log_container_iterator_end();
};

#endif