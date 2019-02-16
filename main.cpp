#include <cstdlib>
#include <ctype.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <iomanip>
#include "IO.hpp"

using namespace std;

extern const std::string src_dir = "/home/shen/Core/Sharp/src/";

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
      statistic();
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

void show_statistic(int day){

}

void statistic() {
  printf("input : (num|d|w|m|y)\n");
  std::string pe;
  while(true){
    std::getline (std::cin,pe);
    if(pe == "q"){
      break;
    }

    else if(pe == ""){
      show_statistic(1);
    }

    else if(pe == "num"){
      // show_statistic(1);
    }
  }
  
}

void add_time_point(string desc, int tag){
  TimeStamp t(desc, tag);
  loader->add_one_record(t);
};

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

void current_task_last(){
  loader->load();
  vector<TimeStamp *> & a = loader->records;
  time_t last = a.back()->cal();
  time_t t = time(nullptr) - last;

  void desktop_notification(const string content, const string icon);
  desktop_notification(time_transform(t), src_dir + "Birdio");
  // cout << time_transform(t) << endl;
}

string exec(const string cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
  if (!pipe)
    throw std::runtime_error("popen() failed!");
  while (!feof(pipe.get())) {
    if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
      result += buffer.data();
  }
  return result;
}

void desktop_notification(const string content,
                                        const string icon) {
  const string quote ="\"";
  string a = "notify-send " + quote + content + quote + "  -i " + icon;
  exec(a);
}
