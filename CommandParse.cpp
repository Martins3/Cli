#include "Handler.hpp"
#include <cstdlib>
#include <ctype.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

using namespace std;

void parse_options(int argc, const char *argv[]) {
  int opt;
  // TODO this line can not change to
  // auto H = Handler::getInstance()
  // how to use auto, what's the meaning of reference

  Handler &H = Handler::getInstance();
  while ((opt = getopt(argc, (char **)argv, "hs:p:")) != EOF) {
    switch (opt) {
    case 's':
      // show statistic
      break;
    case 'p':
      // insert a log
      H.add_time_point(optarg);
      break;
    case 'h':
      printf("usuage is:\n -h show this help information\n -p add one log\n "
             "-t show statistic\n");
      exit(0);
    default:
      exit(1);
    }
  }
}
