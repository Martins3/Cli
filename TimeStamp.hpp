#ifndef TIME_STAMP_HPP
#define TIME_STAMP_HPP

#include "json.hpp"
#include <ctime>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

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
    time_point = temp;
  }

  time_t cal() {
    std::string s = time_point;
    for (int i = 0; i < s.length(); i++) {
      if (s[i] == ':')
        s[i] = ' ';
    }

    // static const char wday_name[][4] = {"Sun", "Mon", "Tue", "Wed",
    // "Thu", "Fri", "Sat"};
    static const char mon_name[][4] = {"Jan", "Feb", "Mar", "Apr",
                                       "May", "Jun", "Jul", "Aug",
                                       "Sep", "Oct", "Nov", "Dec"};

    std::istringstream iss(s);
    struct tm timer={0};
    std::string m;
    iss >> m;
    iss >> m;
    int mon = -1;
    for (int i = 0; i < 12; i++) {
      if (m == mon_name[i]) {
        mon = i;
        break;
      }
    }

    if (mon == -1) {
      assert(0);
    }

    timer.tm_mon = mon;
    iss >> timer.tm_mday;
    iss >> timer.tm_hour;
    iss >> timer.tm_min;
    iss >> timer.tm_sec;
    iss >> timer.tm_year;
    timer.tm_year -= 1900;

    // std::cout << time_point << std::endl;

    struct tm *timeptr = &timer;
    // printf("[%.3s %.3s%3d %.2d:%.2d:%.2d %d\n]",
    // wday_name[timeptr->tm_wday],
    // mon_name[timeptr->tm_mon],
    // timeptr->tm_mday, timeptr->tm_hour,
    // timeptr->tm_min, timeptr->tm_sec,
    // 1900 + timeptr->tm_year);

    auto x = mktime(timeptr);
    if (x == (time_t)-1) {
      assert(0);
    }
    return x;
  }

  TimeStamp() = default;
  // friend std::ostream &operator<<(std::ostream &os, const TimeStamp &t);
};

#endif
