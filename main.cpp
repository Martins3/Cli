#include <cstdlib>
#include <ctype.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include "IO.hpp"

using namespace std;


// output
char statement[] =
    "Please make sure you understand the sharp:\n1. dont't touch phone\n2. "
    "don't leave the sit\n3. don't eat anything\n4. don't let mind wanders\n";

inline void show_help() {
  printf("Specification is:\n"
         "-h show this help information\n"
         "-t show statistic\n"
         "-x terminate current task\n"
         "-a show show how much time consume since last event\n");
}



void parse_options(int argc, const char *argv[]);
void statistic();
void add_time_point(string desc, int tag);
void current_task_last();


Loader * loader;


int main(int argc, const char *argv[]) {
  loader = new Loader();
  parse_options(argc, argv);
  return 0;
}

void parse_options(int argc, const char *argv[]) {
  int opt;
  string desc;

  while ((opt = getopt(argc, (char **)argv, "sax:h")) != EOF) {
    switch (opt) {
    case 's':
      exit(0);
    case 'a':
      current_task_last();
      exit(0);
    case 'x':
      add_time_point(optarg, 1);
      exit(0);
    case 'h':
      show_help();
      exit(0);
    default:
      exit(1);
    }
  }

  // insert a log
  if (argc == 2) {
    add_time_point(argv[1], 0);
    printf("%s", statement);
  } else {
    show_help();
  }
}

void statistic() {
  printf("input : (num|d|w|m|y)\n");
}

void add_time_point(string desc, int tag){
  TimeStamp t(desc, tag);
  loader->add_one_record(t);
};

void current_task_last(){
  // this is complex, my gold
}
