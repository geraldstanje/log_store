#ifndef LOG_FORMATTER_H
#define LOG_FORMATTER_H

#include "log_record.h"
#include <string>

class log_formatter {
  public:
    static std::string to_string(log_record &&record) {
        std::vector<char> vec = record.get_message();
        return std::string(vec.begin(), vec.end());
    }
};

#endif