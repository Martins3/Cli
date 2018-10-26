#include "./TimeStamp.hpp"
#include "../utils/Type.hpp"
#include "Notification.hpp"

using namespace std;

#include <dirent.h>
#include <iterator>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>
#include <vector>

void do_heartbeat(){
   // TODO: implement processing code to be performed on each heartbeat
   TimeStamp t;
   int i = 12;
   for (int i = 0; i < 12; i++) {
       i ++;
       printf("%d\n", i);
   }
}

// For security purposes, we don't allow any arguments to be passed into the daemon
int main(void){
    printf("%s\n", "make it !");
}
