#include "Handler.hpp"
#include "Notification.hpp"
#include "Resource.hpp"
#include "utils/IO.hpp"
#include <cstdlib>
#include <ctype.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

char statement[] =
    "Please make sure you understand the sharp:\n1. dont't touch phone\n2. "
    "don't leave the sit\n3. don't eat anything\n4. don't let mind wanders\n";

// command line
void parse_options(int argc, const char *argv[]);

using namespace std;
int learn_gdb = 100;
int main(int argc, const char *argv[]) {
  parse_options(argc, argv);
  learn_gdb = 200;
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
    case 'f':
      // fix at the last one

    case 'a':
      // show how much time alread used
      H.show_time_last();
      exit(0);
    case 'x':
      exit(0);
    case 'h':
      printf("Specification is:\n"
             "-h show this help information\n"
             "-t show statistic\n"
             "-i Repair by inserting a event in the timeline\n"
             "   Causing by forgetting, format is hour:min:desc\n"
             "-a show show how much time consume since last event\n");
      exit(0);
    default:
      exit(1);
    }
  }

  // insert a log
  if (argc == 2) {
    H.add_time_point(argv[1]);
    printf("%s", statement);
    exit(0);
  }

  // symotion-prefix) test code
  // Loader::store();
}
