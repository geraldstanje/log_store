#include "log_container_iterator.h"
#include "log_record.h"
#include <cstdint>

log_container_iterator::log_container_iterator(log_appender *ptr): ptr_(ptr), curr_record_id_(0) {}

log_container_iterator::log_container_iterator(const log_container_iterator &other) {
    ptr_ = other.ptr_;
    curr_record_id_ = other.curr_record_id_;
}

bool log_container_iterator::operator== (const log_container_iterator &it) {
    return this->get_curr_record_id() == it.get_curr_record_id();
}

bool log_container_iterator::operator!= (const log_container_iterator &it) {
    return this->get_curr_record_id() != it.get_curr_record_id();
}

uint64_t log_container_iterator::get_curr_record_id() const {
    return curr_record_id_;
}

log_record log_container_iterator::operator* () {
    return this->get_curr_record();
}

log_record log_container_iterator::get_curr_record() {
    log_record record;
    if (!ptr_->read_record(curr_record_id_, record)) {
        curr_record_id_ = ptr_->get_end_record_num() - 1;
    }
    return record;
}

void log_container_iterator::operator++(int) {
    curr_record_id_++;
}

void log_container_iterator::operator++() {
    curr_record_id_++;
}

log_container_iterator log_container_iterator::operator+(uint64_t offset) const {
    log_container_iterator it(*this);
    it.log_container_iterator_begin();
    it.curr_record_id_ += offset;
    return it;
}

void log_container_iterator::log_container_iterator_begin() {
    curr_record_id_ = ptr_->get_start_record_num();
}

void log_container_iterator::log_container_iterator_end() {
    curr_record_id_ = ptr_->get_end_record_num();
}