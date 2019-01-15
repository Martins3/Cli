#include <sstream>
#include <iomanip>
#include <iostream>
#include "Handler.hpp"
#include "utils/IO.hpp"
#include "Notification.hpp"
#include "Resource.hpp"

using namespace std;

string time_transform(int seconds){
  int sec = seconds % 60;
  seconds /=60;
  int min = seconds % 60;
  seconds /=60;
  int hour = seconds % 60;

  std::stringstream ss;
  ss << std::setw(2) << std::setfill('0') << hour << ":";
  ss << std::setw(2) << std::setfill('0') << min << ":";
  ss << std::setw(2) << std::setfill('0') << sec;
  return ss.str();
}

void Handler::pop_time_point(){
  Resource & R = Resource::getInstance();
  auto & timeline = R.getTimeline();
  timeline.pop_back();
  Loader::store();
}

void Handler::add_time_point(std::string desc){
  TimeStamp t(desc);
  Loader::add_one_record(t);
}

void Handler::insert_time_point(string event){

}

void Handler::show_time_last(){
  // TODO choose the right record file
  Resource & R = Resource::getInstance();
  auto timeline = R.getTimeline();

#ifdef DEBUG
  cout << "timeline's length is : " << timeline.size() << endl;
#endif

  time_t t = time(nullptr) - timeline.back()->time_point;
  Notification::desktop_notification(time_transform(t), Icon::Birdio);
}
