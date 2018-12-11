#include "Printer.hpp"
#include <algorithm>

using namespace std;

const std::string Printer::ANSI_COLOR_RED     = "\x1b[31m";
const std::string Printer::ANSI_COLOR_GREEN   = "\x1b[32m";
const std::string Printer::ANSI_COLOR_YELLOW  = "\x1b[33m";
const std::string Printer::ANSI_COLOR_BLUE    = "\x1b[34m";
const std::string Printer::ANSI_COLOR_MAGENTA = "\x1b[35m";
const std::string Printer::ANSI_COLOR_CYAN    = "\x1b[36m";
const std::string Printer::ANSI_COLOR_RESET   = "\x1b[0m";

/**
 * str: item name
 * left_indent: widest item's width
 * num: how many number
 */
void Printer::histogram_line(std::string str, int left_indent, int num){
  const char * ptr = str.c_str();
  const int Volume = 50;

  std::cout << ANSI_COLOR_GREEN;
  printf("%*s : ", left_indent,  ptr);

  std::cout << ANSI_COLOR_YELLOW;
  for (int i = 0; i < num; i++) {
    if(i % Volume == 0 && i != 0){
      printf("\n%*c ",left_indent + 2, ':');
    }
    putchar('|');
  }
  printf("\n");
}

void Printer::histogram(std::vector<std::string> & name, std::vector<int> & quantity){
  if(name.size() != quantity.size()){
    std::cerr << "table is incompelete" << std::endl;
    return;
  }

  if(name.empty()){
    std::cerr << "table is empty" << std::endl;
    return;
  }


  int max_indent = 0;
  for(auto & n: name){
    max_indent = std::max(max_indent, (int)n.size());
  }

  for (int i = 0; i < name.size(); i++) {
    histogram_line(name[i], max_indent, quantity[i]);
  }
}

