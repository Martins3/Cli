#ifndef RESOURCE_H
#define RESOURCE_H
#include "TimeStamp.hpp"
#include "utils/IO.hpp"
#include <string>
#include <vector>

// every important message are hold by this class
// it doesn't have coresponding cpp file
class Resource {
  bool timeline_loaded;
  std::vector<TimeStamp *> timeline;
  
public:
  // a bunch of get method to finish
  std::vector<TimeStamp *> &getTimeline() { 
    if(!timeline_loaded){
      timeline_loaded = true;
      Loader::load();
    }
    return timeline;
  }

  // singleton class
public:
  Resource(Resource const &) = delete;
  void operator=(Resource const &) = delete;
  static Resource &getInstance() {
    static Resource instance;
    return instance;
  }

private:
  Resource():timeline_loaded(false){};
};

#endif /* end of include guard: RESOURCE_H */
