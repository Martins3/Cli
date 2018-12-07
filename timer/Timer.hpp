#ifndef TIMER_HPP_PIMMSLOB
#define TIMER_HPP_PIMMSLOB
#include<string>
class Timer{
private:
    bool is_daemon_alive();
    // shutdown yourself at first
    void shutdown_server();
public:
    void start_server();
    void send_message(const std::string msg);
    // start server if it's no active, failed if return not equal to zero
    void start_daemon();
    // start a timer with a event, no more action can be added
    void start();
    void cancel();
    // suspend will be calculated
    void suspend();
    // we should cancel the timer before 
    bool is_timing();
    // show rest time
    int seconds_left();
    // use some really strange method to do it!
};


#endif /* end of include guard: TIMER_HPP_PIMMSLOB */
