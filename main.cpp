#include "log_container.h"
#include "log_container_iterator.h"
#include <iostream>
#include <string>
#include <cassert>

int main() {
    // your code goes here

    std::string out; // used for unit testing

    log_container log("system_log", 520);
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

    return 0;

    // empty log store
    uint64_t size = log.get_position();
    log.truncate(size-1);

    assert(log.get_size() == 0);
    assert(log.check_internal_data() == true);

    return 0;
}