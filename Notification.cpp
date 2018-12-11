#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include "Notification.hpp"
using namespace std;

std::string Notification::exec(const string cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

void Notification::desktop_notification(){
    string a = "notify-send \"who am i\" \"I am January\" -i /home/shen/Core/Sharp/src/GitHub-Mark-64px.png -u critical"; 
    exec(a);
}

void Notification::sound_notification(){
    // use aplay
}


