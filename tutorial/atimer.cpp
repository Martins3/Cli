//
// timer.cpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

void print(const boost::system::error_code& /*e*/)
{
  std::cout << "Hello, world!" << std::endl;
}

void print2(const boost::system::error_code& /*e*/)
{
  std::cout << "Hello, world!" << std::endl;
}

int main()
{
  boost::asio::io_context io;

  boost::asio::deadline_timer t(io, boost::posix_time::seconds(3));
  boost::asio::deadline_timer t2(io, boost::posix_time::seconds(1));
  t.async_wait(&print);
  t2.async_wait(&print2);

  printf("%s\n", "before");

  io.run();

  printf("%s\n", "after");

  return 0;
}
