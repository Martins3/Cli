#include "./TimeStamp.hpp"

TimeStamp::TimeStamp(){
    time = std::chrono::system_clock::now();
}
