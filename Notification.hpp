#ifndef NOTIFICATION_HPP
#define NOTIFICATION_HPP
#include<string>

class Notification{
public:
  static std::string exec(const std::string cmd);
  static void desktop_notification();
  static void test_exec();
  static void sound_notification();
};
#endif
