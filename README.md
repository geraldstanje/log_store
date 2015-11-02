## Log Store

# Info
A log store is a record storage model where updates only need to append rather than seeking and writing. To load the records, the log store is read from a position and replayed back. The ordering of records ensure a consistent model. A log store would grow without bound unless it is maintained. One way reduce its size is to re-append the records to be preserved, then truncate the store. Truncating the store should release storage back to the operating system. However, not all data is required to be released, so long as the disk footprint remains roughly proportional to the data retained.

In C++ or C++11 (or higher), implement a simple log store with the following requirements and functions:

# Requirements

The implementation must be thread safe
Records are variably sized opaque blobs of bytes anywhere from 1 byte to 32MB
The log store must store on disk (not an in-memory log store)
Truncation must be efficient and not require too much copying of data

Functions:

  append(blob) -> position
    Append a record blob to the log store.

  get_position() -> position
    Gets the current position of the log store.

  truncate(position)
    Removes all data before the given position in the log store. Disk space used must remain roughly proportional to retained data size.

  replay(position, callback)
    Replays all record blobs from the position onward, invoking the callback for each blob visited

Note: The above function signatures are only illustrative. You may vary your interface in whatever way you think makes sense. For example, instead of a callback for replay (...), you may decide to use an iterator pattern.

Notes:

You may use the standard C and C++ libraries, as well as any system calls available on the OS. Libraries such as boost are also acceptable. However, your homework will be judged on how much of the problem your code solves versus how much is solved by libraries.

# Design

## Supported Platforms
- Mac: OS X 10.10.3 (Yosemite) or greater, x86 and x86-64

## Compiler
- Clang++ <br>
Apple LLVM version 6.1.0 (clang-602.0.53) (based on LLVM 3.6.0svn)
Target: x86_64-apple-darwin14.3.0
Thread model: posix

## General
- to minimize copying, blobs will segmented into multiple files
- we have files F_x... F_n and they are filled with data, x = start_record_id_, n = end_record_id_
- the log store internally maintains a 64 bit start_record_id_ and 64 bit end_record_id_
- append will create a new file (*.data) with the current end_record_id_ and increment end_record_id_ plus 1
- truncate will find all the files before the given position that can be deleted, start_record_id_ will be incremented to the position before the files will be delted.
- the log store destructor exports the 64 bit start_record_id_ and 64 bit end_record_id_ to a .config file, which can be read at startup

## Multithreading
  1. replay a record X: lock; get first number F; open file X+F; unlock; read file <br>
  -> we can unlock before reading the file. if the file is open unlink can remove the file once all open file handles are closed 
  2. append a new record: write to tmp file; lock; increment record_current_index number L; rename file; unlock <br>
  -> we create a tmp file because writing is relatively long and we do not want to keep the lock the whole time; instead we lock just for the duration of rename <br>
  -> rename is also relativevy slow; but not nearly as slow as writing several megabytes
  3. truncate at P; lock; remember old first F; set new first to P; unlock; proceed to delete files with numbers less than F

# Example Usage
```cpp
// create log container
log_container log("system_log");

// append blobs
log.append(string_record("hello_how_are_you"));
log.append(string_record("_great"));
log.append(string_record("_amazing"));
log.append(string_record("_yes"));

// replay blobs from the beginning till end
for (log_container_iterator itr = log.begin(); itr != log.end(); itr++) {
    std::cout << *itr << std::endl;
}

// truncate blobs from a position till the start
uint64_t pos = log.get_position();
log.truncate(pos-1);
```

# Run Unit test
The following command runs a unit test.

$ python compile_and_test.py

# TODO:
- check code with clang static analyzer
- add more testing
- add different log record types, at the moment only string records are supported
- initialize start_record_id_ and end_record_id_ to zero if they hit uint64_t::max
- replace function 'tmpnam' which is deprecated (compiler warning) in src/util.cpp