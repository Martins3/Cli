#include <assert.h>
#include <cstdlib>
#include <ctype.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

#include "IO.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

using namespace std;

const std::string ANSI_COLOR_RED = "\x1b[31m";
const std::string ANSI_COLOR_GREEN = "\x1b[32m";
const std::string ANSI_COLOR_YELLOW = "\x1b[33m";
const std::string ANSI_COLOR_BLUE = "\x1b[34m";
const std::string ANSI_COLOR_MAGENTA = "\x1b[35m";
const std::string ANSI_COLOR_CYAN = "\x1b[36m";
const std::string ANSI_COLOR_RESET = "\x1b[0m";

inline int get_pos_number_from_user() {
  printf("Postive number: ");
  int a;
  while (true) {
    if (scanf("%d", &a) == 1 && a > 0) {
      return a;
    }
  }
}

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

void show_file(string file_name);
struct tm *get_current_time() {
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  return timeinfo;
}

Loader *loader;

string time_transform(int seconds);
void parse_options(int argc, const char *argv[]);
void statistic();
void add_time_point(string desc, int tag);
// void current_task_last();
void desktop_notification(const string content, const string icon);

std::pair<time_t, bool> get_task_last() {
  loader->load();
  vector<TimeStamp *> &a = loader->records;
  auto ts = a.back();
  time_t last = ts->cal();
  time_t t = time(nullptr) - last;
  return make_pair(t, ts->tag);
}

int main(int argc, const char *argv[]) {
  loader = new Loader();
  parse_options(argc, argv);
  return 0;
}

void edit_file(const char *file) {
  printf("start nvim !\n");
  char path[200];
  strcpy(path, file);
  char program[] = "nvim";
  char *args[] = {program, path, NULL};
  int status;
  if (!fork()) {
    execvp("nvim", args);
  }
  wait(&status);
}

void edit_source() { edit_file((src_dir + "record.json").c_str()); }

void review() {
  printf("Welcome to review (d|w|m|y|s|t)\n");
  char option;
  scanf("%c", &option);

  auto cur = get_current_time();
  auto file_name = src_dir + "introspection/";
  int y = cur->tm_year + 1900;
  int m = cur->tm_mon + 1;
  int d = cur->tm_mday;

  switch (option) {
  case '\n':
  case 'd':
    file_name += to_string(y) + "/" + to_string(m) + "/" + to_string(d) + ".md";
    break;
  case 'w':
    // week is tricky
    file_name +=
        to_string(y) + "/" + to_string(m) + "/Week/" + to_string(d) + ".md";
    break;
  case 'm':
    file_name += to_string(y) + "/" + to_string(m) + ".md";
    break;
  case 'y':
    file_name += to_string(y) + ".md";
    break;
  case 's':
    file_name += "summary/";
    break;
  case 't':
    file_name = src_dir + "todos.md";
    break;
  default:
    printf("check the input");
  }
  edit_file(file_name.c_str());
}

void show_already() {
  auto x = get_task_last();
  auto t = x.first;
  auto work = x.second;
  string msg;
  if (work) {
    msg += "已休息 : ";
  } else {
    msg += "已工作 : ";
  }

  msg += time_transform(t);
  desktop_notification(msg, src_dir + "Birdio");
}

void remove_todo_item() {
  // read all the items
  show_file("todos.md");
  int index = get_pos_number_from_user() - 1;
  std::ifstream infile(src_dir + "todos.md");
  std::string line;
  vector<string> items;

  int line_num = 0;
  while (std::getline(infile, line)) {
    std::istringstream iss(line);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                     std::istream_iterator<std::string>());
    if(results.size() != 2){
      printf("%d the todo format: [A. B] A is num B shouldn't contains any space", line_num);
      continue;
    }
    items.push_back(results[1]);
    line_num ++;
  }
  infile.close();

  // write excpet the index
  std::ofstream log(src_dir + "todos.md", std::ios_base::out);
  for (int i = 0; i < items.size(); i++) {
    if (i == index)
      continue;
    log << i + 1 << ". ";
    log << items[i] << endl;
  }
}

void add_todo_item(string a) {
  if (a == "f") {
    remove_todo_item();
    return;
  }
  std::ifstream infile(src_dir + "todos.md");
  std::string line;
  int index = 0;
  while (std::getline(infile, line)) {
    index++;
  }
  std::ofstream log(src_dir + "todos.md",
                    std::ios_base::app | std::ios_base::out);
  log << index << ". ";
  log << a << endl;
}

void parse_options(int argc, const char *argv[]) {
  int opt;
  string desc;

  while ((opt = getopt(argc, (char **)argv, "sax:hert:")) != EOF) {
    switch (opt) {
    case 's':
      statistic();
      exit(0);
    case 'r':
      review();
      exit(0);
    case 'a':
      show_already();
      exit(0);
    case 't':
      add_todo_item(optarg);
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
    auto t = get_current_time();
    void show_file(string);
    show_file("Regulation.md");
    // printf("%s", statement);
    printf("start : %02d:%02d\n", t->tm_hour, t->tm_min);

  } else {
    show_help();
  }
}

void show_file(string file_name) {
  std::string line;
  std::ifstream infile(src_dir + file_name);
  cout << ANSI_COLOR_RED;

  while (std::getline(infile, line)) {
    cout << line << endl;
  }
  cout << ANSI_COLOR_YELLOW;
}

time_t n_days_ago(int day) {
  // time_t rawtime;
  // struct tm *timeinfo;
  // time(&rawtime);
  auto timeinfo = get_current_time();
  if (day != 1) {
    timeinfo->tm_mday = timeinfo->tm_mday - day;
  } else {
    timeinfo->tm_hour = 0;
    timeinfo->tm_min = 0;
    timeinfo->tm_sec = 0;
  }

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

    cout << ANSI_COLOR_BLUE << start->time_hour_min() << "----->"
         << end->time_hour_min() << ANSI_COLOR_YELLOW << " "
         << time_transform(r - l) << "  " << ANSI_COLOR_CYAN << start->desc
         << "  " << ANSI_COLOR_GREEN << end->desc << endl;
  }

  if (day != 1) {
    printf("recent %d day: %s", day, time_transform(sum).c_str());
  } else {
    time_t threshold = 60 * 60 * 8;
    cout << ANSI_COLOR_MAGENTA << "today : ";
    if (sum < threshold) {
      cout << ANSI_COLOR_RED;
    } else {
      cout << ANSI_COLOR_YELLOW;
    }
    cout << time_transform(sum);
    // printf("today: %s", time_transform(sum).c_str());
  }
}

void statistic() {
  printf("input : n(um)|d|w|m|y\n");
  std::string pe;
  char option;
  scanf("%c", &option);
  switch (option) {
  case '\n':
  case 'd':
    show_statistic(1);
    break;
  case 'w':
    show_statistic(7);
    break;
  case 'm':
    show_statistic(30);
    break;
  case 'y':
    show_statistic(365);
    break;
  case 'n':
    show_statistic(get_pos_number_from_user());
    break;
  default:
    printf("Unrecognized input");
  }
}

void add_time_point(string desc, int tag) {
  auto x = get_task_last();
  auto t = x.first;
  auto work = x.second;

  if (tag == 1 && work == 0) {
    cout << "已工作 : " << time_transform(t) << endl;
    cout << "还有如下可以尝试的事情 : " << endl;
    show_file("todos.md");
  } else if ((tag ^ work) == 0) {
    cout << ANSI_COLOR_RED << "Records are not well paired !" << endl;
    return;
  }

  TimeStamp ts(desc, tag);
  loader->add_one_record(ts);
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
