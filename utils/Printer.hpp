#ifndef PRINTER_H
#define PRINTER_H

#include <iostream>
#include <unistd.h>
#include <iomanip>
#include <vector>


class Printer{
    static const std::string ANSI_COLOR_RED     ;
    static const std::string ANSI_COLOR_GREEN   ;
    static const std::string ANSI_COLOR_YELLOW  ;
    static const std::string ANSI_COLOR_BLUE    ;
    static const std::string ANSI_COLOR_MAGENTA ;
    static const std::string ANSI_COLOR_CYAN    ;
    static const std::string ANSI_COLOR_RESET   ;

    static void histogram_line(std::string str, int left_indent, int num);
public:
    static void histogram(std::vector<std::string> & name, std::vector<int> & quantity);
};

#endif /* end of include guard: PRINTER_H */
