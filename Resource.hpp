#ifndef RESOURCE_H
#define RESOURCE_H
#include "TimeStamp.hpp"
#include "utils/IO.hpp"
#include <string>
#include <vector>

/** 1. every important message are hold by this class
  * 2. it doesn't have coresponding cpp file
  * 3. this is the only class interact with loader
  */


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

  void saveTimeline(){
    Loader::store();
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

#endif
/* end of include guard: RESOURCE_H */
