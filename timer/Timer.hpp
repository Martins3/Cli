#ifndef TIMER_HPP_PIMMSLOB
#define TIMER_HPP_PIMMSLOB

class Timer{
public:
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
    // t->expires_at(t->expires_at() + boost::posix_time::seconds(1));
};


#endif /* end of include guard: TIMER_HPP_PIMMSLOB */
