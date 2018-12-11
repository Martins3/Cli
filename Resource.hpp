#ifndef RESOURCE_H
#define RESOURCE_H
#include "TimeStamp.hpp"
#include <string>
#include <vector>

// every important message are hold by this class
// it doesn't have coresponding cpp file
class Resource {
  std::vector<TimeStamp *> timelines;

public:
  // a bunch of get method to finish
  std::vector<TimeStamp *> &getTimeline() { return timelines; }

  // singleton class
public:
  Resource(Resource const &) = delete;
  void operator=(Resource const &) = delete;
  static Resource &getInstance() {
    static Resource instance;
    return instance;
  }

private:
  Resource()=default;
};

#endif /* end of include guard: RESOURCE_H */
