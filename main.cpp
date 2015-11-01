/*
- to minimize copying, we want to have it segmented in multiple files
- and 'truncate' will find all the files before the given position that are fully of
completely deletable data
- i would have files F_1... F_n and they are filled with lines of data that have
corresponding 'position' IDs
- the main decision we have to make is how you will put the data into the files
- how many files is we max / how big is the max size on a file before you move to the next
- and then how you will expire files that are before the 'position' supplied
- there is a limit to the number of FDs the program can have open
- FDs are a kernel resource, on OS X it is 256 FDs
- ulimit -a lists it
- add a record_index at the beginning of each file
- multithreading concept:
  reading a record X: lock; get first number F; open file X+F; unlock; read file
  writing a new record: write to tmp file; lock; increment last number L; rename file; unlock
  -> tmp file because writing is relatively long and you don't want to keep the lock the whole time; instead you lock just for the duration of rename
  -> rename is also relativevy slow; but not nearly as slow as writing several megabytes
  truncating at P; lock; remember old first F; set new first to P; unlock; proceed to delete files with numbers less than F
*/

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

    // empty log store
    uint64_t size = log.get_position();
    log.truncate(size-1);

    assert(log.get_size() == 0);
    assert(log.check_internal_data() == true);

    return 0;
}