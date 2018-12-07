#include "../Timer.hpp"
#include <cstring>

int main(int argc, const char *argv[]){
  Timer timer;
  if(argc != 2){
    return 0;
  }

  if(strcmp(argv[1], "client") == 0){
    timer.send_message("hhh");
  }

  else if(strcmp(argv[1], "server") == 0){
    timer.start_server();
  }

  else if(strcmp(argv[1], "daemon") == 0){
    timer.start_daemon();
  }
  return 0;
}
