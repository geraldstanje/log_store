#include "log_container.h"
#include "log_container_iterator.h"
#include "log_formatter.h"
#include <iostream>

int main() {
    log_container log("system_log");
    log.append(string_record("hello_how_are_you"));
    log.append(string_record("_great"));
    log.append(string_record("_amazing"));
    log.append(string_record("_yes"));

    for (log_container_iterator itr = log.begin()+1; itr != log.end(); itr++) {
        std::cout << log_formatter::format(*itr) << std::endl;
    }

    // empty log store
    uint64_t pos = log.get_position();
    log.truncate(pos-1);

    return 0;
}