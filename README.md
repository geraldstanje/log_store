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

## General
- to minimize copying, blobs will segmented into multiple files
- truncate will find all the files before the given position that are fully of completely deletable data
- we have files F_1... F_n and they are filled with lines of data that have corresponding 'position' IDs
- the log store internally maintains a 64 bit record_start_index and 64 bit record_current_index
- *.log stores the 64 bit record_start_index and 64 bit record_current_index
- *.data stores the logged data, each blob will be stored in a separated file

## Multithreading concept
  1. replay a record X: lock; get first number F; open file X+F; unlock; read file
  2. append a new record: write to tmp file; lock; increment record_current_index number L; rename file; unlock
  -> we create a tmp file because writing is relatively long and we do not want to keep the lock the whole time; instead we lock just for the duration of rename
  -> rename is also relativevy slow; but not nearly as slow as writing several megabytes
  3. truncate at P; lock; remember old first F; set new first to P; unlock; proceed to delete files with numbers less than F