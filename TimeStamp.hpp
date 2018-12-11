#ifndef TIME_STAMP_HPP
#define TIME_STAMP_HPP 

#include <ctime>
#include <string>
#include "./utils/json.hpp"

// data type
class TimeStamp{
public:
    time_t time_point;
    std::string desc;
    int tag; // tag, use to classify items
    TimeStamp(std::string d):time_point(time(nullptr)), desc(d){}
    TimeStamp()=default;
};
#endif
