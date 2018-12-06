#ifndef TIME_STAMP_HPP
#define TIME_STAMP_HPP 

#include <numeric>
#include <chrono>
#include <string>

// data type
class TimeStamp{
public:
    std::chrono::time_point<std::chrono::system_clock> time;
    std::string desc;
    int tag; // tag for statistic
    TimeStamp(std::string d):time(std::chrono::system_clock::now()), desc(d){}
};
#endif
