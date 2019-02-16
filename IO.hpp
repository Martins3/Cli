#ifndef IO_HPP_9FT0KANB
#define IO_HPP_9FT0KANB

#include<string>
#include<vector>
#include "TimeStamp.hpp"

class Loader{
public:
  std::vector<TimeStamp *> records;
  void load();
  void store();
  void add_one_record(TimeStamp & t);
};


#endif /* end of include guard: IO_HPP_9FT0KANB */
