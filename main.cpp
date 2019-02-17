#include <assert.h>
#include <cstdlib>
#include <ctype.h>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>

#include "IO.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

using namespace std;

static inline void data_broken(int msg) {
  printf("We found events are not well paired ! Use sharp -e to check data\n");
  if (msg != 0)
    printf("Break line is %d\n", msg);

  exit(0);
}
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

string exec(const string cmd);

string time_transform(int seconds);
void parse_options(int argc, const char *argv[]);
void statistic();
void add_time_point(string desc, int tag);
// void current_task_last();
void desktop_notification(const string content, const string icon);
time_t get_task_last();

Loader *loader;

int main(int argc, const char *argv[]) {
  loader = new Loader();
  parse_options(argc, argv);
  return 0;
}

void edit_source() {
  printf("start vim !\n");
  char path[200];
  strcpy(path, (src_dir + "record.json").c_str());
  char program[] = "nvim";
  char *args[] = {program, path, NULL};
  int status;
  if (!fork()) {
    execvp("nvim", args);
  }
  wait(&status);
}

void parse_options(int argc, const char *argv[]) {
  int opt;
  string desc;

  while ((opt = getopt(argc, (char **)argv, "sax:he")) != EOF) {
    switch (opt) {
    case 's':
      statistic();
      exit(0);
    case 'a':
      desktop_notification(time_transform(get_task_last()), src_dir + "Birdio");
      exit(0);
    case 'x':
      add_time_point(optarg, 1);
      exit(0);
    case 'h':
      show_help();
      exit(0);
    case 'e':
      edit_source();
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

time_t n_days_ago(int day) {
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  timeinfo->tm_mday = timeinfo->tm_mday - 1;

  auto x = mktime(timeinfo);
  if (x == (time_t)-1) {
    assert(0);
  }
  return x;
}

void show_statistic(int day) {
  loader->load();
  vector<TimeStamp *> &a = loader->records;
  if (a.size() % 2 != 0) {
    data_broken(0);
  }

  if (a.empty()) {
    printf("Empty database, start to record your life now !");
    return;
  }

  auto limitation = n_days_ago(day);
  // vector<TimeStamp *> b;

  time_t sum = 0;

  int i;
  for (i = a.size() - 2; i >= 0; i -= 2) {
    auto start = a[i];
    auto end = a[i + 1];
    auto l = start->cal();
    auto r = end->cal();
    if (start->tag != 0 || end->tag != 1 || l > r) {
      data_broken(i);
    }

    if (l > limitation) {
      sum += (r - l);
    } else {
      break;
    }

    cout << start->time_hour_min() << "----->" << end->time_hour_min() << " "
         << time_transform(r - l) << "  " << start->desc << "  " << end->desc << endl;
  }

  if (day != 1) {
    printf("recent %d day: %s", day, time_transform(sum).c_str());
  } else {
    printf("today: %s", time_transform(sum).c_str());
  }
}

void statistic() {
  printf("input : (num|d|w|m|y)\n");
  std::string pe;
  while (true) {
    std::getline(std::cin, pe);
    if (pe == "q") {
      break;
    }

    else if (pe == "") {
      show_statistic(1);
      break;
    }

    else if (pe == "num") {
      // show_statistic(1);
    }

    else {
      printf("Unrecognized command !\n");
    }
  }
}

void add_time_point(string desc, int tag) {
  if (tag == 1) {
    cout << "last : " << time_transform(get_task_last()) << endl;
  }

  TimeStamp t(desc, tag);
  loader->add_one_record(t);
};

string time_transform(int seconds) {
  int sec = seconds % 60;
  seconds /= 60;
  int min = seconds % 60;
  seconds /= 60;
  int hour = seconds % 60;

  std::stringstream ss;
  ss << std::setw(2) << std::setfill('0') << hour << ":";
  ss << std::setw(2) << std::setfill('0') << min << ":";
  ss << std::setw(2) << std::setfill('0') << sec;
  return ss.str();
}

time_t get_task_last() {
  loader->load();
  vector<TimeStamp *> &a = loader->records;
  time_t last = a.back()->cal();
  time_t t = time(nullptr) - last;
  return t;
}

void current_task_last() {
  // cout << time_transform(t) << endl;
}

string exec(const string cmd) {
  // cout << cmd;
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

void desktop_notification(const string content, const string icon) {
  const string quote = "\"";
  string a = "notify-send " + quote + content + quote + "  -i " + icon;
  exec(a);
}
