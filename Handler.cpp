#include "Handler.hpp"
#include "utils/IO.hpp"

using namespace std;

void Handler::add_time_point(std::string desc){
  TimeStamp t(desc);
  Loader::add_one_record(t);
}
