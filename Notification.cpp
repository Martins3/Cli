#include "Notification.hpp"
#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
using namespace std;

std::string Notification::exec(const string cmd) {
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

void Notification::desktop_notification(const string content,
                                        const string icon) {
  const string quote ="\"";
  string a = "notify-send " + quote + content + quote + "  -i " + icon;
  exec(a);
}

void Notification::sound_notification() {
  // use aplay
}
