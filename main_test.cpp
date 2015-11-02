#include "log_container.h"
#include "log_container_iterator.h"
#include "util.h"
#include <iostream>
#include <string>
#include <cassert>
#include <thread>
#include <chrono>
#include <sstream>
#include <mutex>

class thread_test {
  private:
    log_container *log_;
    std::mutex lock_;
    std::vector<std::vector<std::string>> out;

  public:
    thread_test(log_container *log): log_(log) {}

    void writer_thread() {
        for (uint64_t i = 0; i < 100; i++) {
            std::stringstream ss;
            ss << i;
            log_->append(string_record("hello_world: " + ss.str()));

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    void read_thread() {
        std::vector<std::string> records;

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        for (log_container_iterator itr = log_->begin(); itr != log_->end(); itr++) {
            records.push_back(*itr);
        }

        std::lock_guard<std::mutex> lock(lock_);
        out.push_back(records);
    }

    std::vector<std::vector<std::string>> get_out() const {
        return out;
    }
};

bool check_read_data(std::vector<std::vector<std::string>> &&out) {
    std::string pattern = "hello_world: ";

    for (auto &vec : out) {
        int64_t prev_count = -1;

        for (auto &e : vec) {
            size_t pos = e.find(pattern);
            if (pos == std::string::npos) {
                return false;
            }

            pos += pattern.size();
            std::string counter = e.substr(pos, e.size() - pos);
            int64_t count = string_to_int(counter);

            if (count <= prev_count) {
                return false;
            }

            prev_count = count;
        }
    }

    return true;
}

void multithreading_test() {
    log_container log("system_log");
    thread_test t(&log);

    std::vector<std::thread> reader_pool;

    // create writer thread
    std::thread writer(&thread_test::writer_thread, &t);

    // create multiple reader threads
    for (uint64_t i = 0; i < 10; i++) {
        reader_pool.push_back(std::thread(&thread_test::read_thread, &t));
    }

    // wait for all threads to finish
    writer.join();
    for (auto& thread : reader_pool) {
        thread.join();
    }

    // check read data
    assert(check_read_data(t.get_out()) == true);

    // empty log store
    uint64_t pos = log.get_position();
    log.truncate(pos-1);

    assert(log.get_size() == 0);
}

void basic_test() {
    std::string out;

    log_container log("system_log");
    log.append(string_record("hello_how_are_you"));
    log.append(string_record("_great"));
    log.append(string_record("_amazing"));
    log.append(string_record("_yes"));

    assert(log.get_size() == 4);

    out.clear();

    for (log_container_iterator itr = log.begin(); itr != log.end(); itr++) {
        out += *itr;
    }

    assert(out == "hello_how_are_you_great_amazing_yes");

    log.truncate(0);
    assert(log.get_size() == 3);

    out.clear();
    for (log_container_iterator itr = log.begin(); itr != log.end(); itr++) {
        out += *itr;
    }

    assert(out == "_great_amazing_yes");

    log.append(string_record("_wonderful"));

    assert(log.get_size() == 4);

    out.clear();
    for (log_container_iterator itr = log.begin(); itr != log.end(); itr++) {
        out += *itr;
    }

    assert(out == "_great_amazing_yes_wonderful");

    // empty log store
    uint64_t pos = log.get_position();
    log.truncate(pos-1);

    assert(log.get_size() == 0);
}

int main() {

    basic_test();
    multithreading_test();

    return 0;
}