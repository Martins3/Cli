#include "Notification.hpp"
#include "utils/IO.hpp"
#include "Handler.hpp"
#include "Resource.hpp"
#include <cstdlib>
#include <ctype.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

// command line
void parse_options(int argc, const char *argv[]);

using namespace std;
int main(int argc, const char *argv[]) {
  parse_options(argc, argv);
  return 0;
}

void parse_options(int argc, const char *argv[]) {
  int opt;
  // TODO this line can not change to
  // auto H = Handler::getInstance()
  // how to use auto, what's the meaning of reference
  Handler &H = Handler::getInstance();
  Resource &R = Resource::getInstance();
  while ((opt = getopt(argc, (char **)argv, "hs:p:ai:")) != EOF) {
    switch (opt) {
    case 's':
      // show statistic
      // TODO this part is difficult, make a better judgement for it
      exit(0);
    case 'i':
      H.insert_time_point(optarg);
      exit(0);
    case 'a':
      // show how much time alread used
      H.show_time_last();
      exit(0);
    case 'h':
      printf("Specification is:\n"
             "-h show this help information\n"
             "-t show statistic\n"
             "-i Repair by inserting a event in the timeline\n"
             "   Causing by forgetting, format is hour:min:desc\n"
             "-a show show how much time consume since last event\n"
             );
      exit(0);
    default:
      exit(1);
    }
  }

  // insert a log
  if(argc == 2){
    H.add_time_point(argv[1]);
    exit(0);
  }

  //symotion-prefix) test code
  // Loader::store();
}
