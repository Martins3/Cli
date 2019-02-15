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

inline void show_help() {
  printf("Specification is:\n"
         "-h show this help information\n"
         "-t show statistic\n"
         "-i Repair by inserting a event in the timeline\n"
         "-a show show how much time consume since last event\n");
}

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
  // Resource &R = Resource::getInstance();

  int minutes;
  string desc;
  int fix_insertion = 0;

  while ((opt = getopt(argc, (char **)argv, "hasd:t:")) != EOF) {
    switch (opt) {
    case 's':
      // show statistic
      // TODO this part is difficult, make a better judgement for it
      exit(0);
    case 't':
      minutes = strtol(optarg, NULL, 10);
      if (minutes == errno) {
        std::cerr << "word id should be a integer" << std::endl;
        exit(1);
      }
      fix_insertion++;
      break;
    case 'd':
      desc = optarg;
      fix_insertion++;
      break;
    case 'a':
      H.show_time_last();
      exit(0);
    case 'x':
      H.pop_time_point();
      exit(0);
    case 'h':
      show_help();
      exit(0);
    default:
      exit(1);
    }
  }

  if (fix_insertion == 2) {
    H.insert_time_point(desc, minutes);
    exit(0);
  }

  if (fix_insertion == 1) {
    std::cerr << "option t and option d should appear same time" << endl;
    exit(1);
  }

  // insert a log
  if (argc == 2) {
    H.add_time_point(argv[1]);
    printf("%s", statement);
    exit(0);
  }

  if (argc == 1) {
    show_help();
  }
  // symotion-prefix) test code
  // Loader::store();
}
