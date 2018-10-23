#include "./TimeStamp.hpp"
#include "../utils/Type.hpp"
#include "Notification.hpp"

using namespace std;

int main(int argc, const char *argv[]){
    string a = "notify-send \"who am i\" \"I am January\" -i /home/shen/Downloads/GitHub-Mark/PNG/GitHub-Mark-64px.png"; 
    exec(a);
    return 0;
}
