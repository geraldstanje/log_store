#include "log_container.h"
#include "log_container_iterator.h"

int main() {
    // your code goes here

    log_container log("system_log", 520);
    log.append(string_record("hello_how_are_you"));
    log.append(string_record("_great"));
    log.append(string_record("_amazing"));
    log.append(string_record("_yes"));

    return 0;
}