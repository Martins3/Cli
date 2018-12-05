#ifndef TIME_STAMP_HPP
#define TIME_STAMP_HPP 

#include <iostream>
#include <vector>
#include <numeric>
#include <chrono>
#include <string>
#include "./utils/Type.hpp"

// data type
class TimeStamp{
public:
    std::chrono::time_point<std::chrono::system_clock> time;
    std::string desc;
    TimeStamp();
};
#endif
