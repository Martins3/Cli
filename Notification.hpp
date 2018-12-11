#ifndef NOTIFICATION_HPP
#define NOTIFICATION_HPP
#include<string>

// TODO when there are just static, why not use namespace instead of class
namespace Icon{
  const std::string Birdio = "Birdio.png";
}

class Notification{
public:
  static std::string exec(const std::string cmd);
  static void desktop_notification(const std::string content, const std::string icon);
  static void test_exec();
  static void sound_notification();
};
#endif
