#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>

// https://www.boost.org/doc/libs/1_45_0/doc/html/boost_asio/reference/deadline_timer.html

/**
void on_timeout(const boost::system::error_code &e) {
  if (e != boost::asio::error::operation_aborted) {
    // Timer was not cancelled, take necessary action.
    std::cout << "By my will, this shall be finished" << std::endl;

  } else {
    std::cout << "Cancel it !" << std::endl;
  }
}
*/

class printer {
public:
  printer(boost::asio::io_context &io)
      : timer_(io, boost::posix_time::seconds(1000)), valid(true) {
    timer_.async_wait(boost::bind(&printer::on_timeout, this));
  }

  // ~printer()
  // {
  // std::cout << "Final count is " << count_ << std::endl;
  // }

  // set a new timer
  //
  void query() { std::cout << timer_.expires_at() << std::endl; }

  void prolong(int seconds) {
    // the new set a new timer can cancel fomer one
    if (timer_.expires_from_now(boost::posix_time::seconds(1)) > 0) {
      valid = false;
      // We managed to cancel the timer. Start new asynchronous wait.
      // timer_.async_wait(on_timeout);
      // timer_.cancel();
      // valid = false;
      // timer_.expires_at(timer_.expires_at() +
      // boost::posix_time::seconds(seconds));
      // timer_.async_wait(boost::bind(&printer::on_timeout, this));
      // timer_.async_wait(::on_timeout);
      // timer_.expires_from_now(boost::posix_time::seconds(1));
      timer_.async_wait(boost::bind(&printer::on_timeout, this));

      std::cout << "Added five more seconds" << std::endl;
      // std::cout << "Timer expire at " << timer_.expires_at() << std::endl;
      // prolong(5);
    } else {
      std::cout << "It's already over " << std::endl;
      // Too late, timer has already expired!
      // timer_.expires_from_now(boost::posix_time::seconds(1));
      // timer_.async_wait(::on_timeout);
      // timer_.async_wait(boost::bind(&printer::on_timeout, this));
      // std::cout << "I will create a new one" << std::endl;
    }
  }

  void on_timeout() {
    if (valid) {
      // Timer was not cancelled, take necessary action.
      std::cout << "By my will, this shall be finished" << std::endl;
      // timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(1));
      timer_.expires_from_now(boost::posix_time::seconds(1000));
      timer_.async_wait(boost::bind(&printer::on_timeout, this));
    } else {
      valid = true;
      std::cout << "Cancel it !" << std::endl;
    }
  }

private:
  boost::asio::deadline_timer timer_;
  bool valid;
};

int main() {
  boost::asio::io_context io;
  printer p(io);
  // io.run();
  boost::thread(boost::bind(&boost::asio::io_context::run, &io)).detach();

  std::string line;
  while (getline(std::cin, line)) {
    if (line == "gg") {
      break;
    } else {
      p.prolong(5);
    }
  }
  return 0;
}
