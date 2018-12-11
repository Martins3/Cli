#include "Notification.hpp"
#include "./utils/IO.hpp"

// command line
void parse_options(int argc, const char *argv[]);

using namespace std;
int main(int argc, const char *argv[]) {
  parse_options(argc, argv);
  return 0;
}

void test_notification(){
  // Notification::desktop_notification();
  vector<TimeStamp *> a;
  Loader::store(a);
}
