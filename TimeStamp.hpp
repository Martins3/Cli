#ifndef TIME_STAMP_HPP
#define TIME_STAMP_HPP

#include "json.hpp"
#include <ctime>
#include <stdio.h>
#include <string>
#include <iostream>

class TimeStamp {
public:
  std::string time_point;
  std::string desc;
  int tag;
  TimeStamp(std::string d, int t) : desc(d), tag(t) {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char temp[] = "Wed Feb 13 15:46:11 2013";
    sprintf(temp, "%s", asctime(timeinfo));
    d = temp;
    std::cout << d << std::endl;
  }

  TimeStamp() = default;
  // friend std::ostream &operator<<(std::ostream &os, const TimeStamp &t);
};

#endif
