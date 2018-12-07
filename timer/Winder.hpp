#ifndef WINDER_H
#define WINDER_H
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>

class Winder {
public:
  Winder(boost::asio::io_context &io)
      : timer_(io, boost::posix_time::seconds(INT_MAX)), valid(true) {
    timer_.async_wait(boost::bind(&Winder::on_timeout, this));
  }

  void query() { std::cout << timer_.expires_at() << std::endl; }

  void set(int seconds) {
    if (timer_.expires_from_now(boost::posix_time::seconds(seconds)) > 0) {
      valid = false;
      timer_.async_wait(boost::bind(&Winder::on_timeout, this));
    }
  }

  void on_timeout() {
    if (valid) {
      // TODO make a notification

      timer_.expires_from_now(boost::posix_time::seconds(INT_MAX));
      timer_.async_wait(boost::bind(&Winder::on_timeout, this));
    } else {
      valid = true;
    }
  }

private:
  boost::asio::deadline_timer timer_;
  bool valid;
};



#endif /* end of include guard: WINDER_H */
