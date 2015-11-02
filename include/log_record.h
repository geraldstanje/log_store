#ifndef LOG_RECORD_H
#define LOG_RECORD_H

#include <vector>
#include <string>

// a record could be a string-record, time-record, object-record,
// holds internally a std::vector<char>
class log_record {
  private:
    std::vector<char> message_;

  public:
    log_record(std::vector<char> vec);
    std::vector<char> get_message() const;
};

class string_record: public log_record {
  public:
    string_record(const std::string &str);
};

#endif