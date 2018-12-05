//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <iostream>

using boost::asio::ip::tcp;
void print(const boost::system::error_code& /*e*/)
{
  std::cout << "Hello, world!" << std::endl;
}

int go_main()
{
  boost::asio::io_context io;

  boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
  t.async_wait(&print);

  io.run();

  return 0;
}


std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

class tcp_connection
  : public boost::enable_shared_from_this<tcp_connection>
{
public:
  typedef boost::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_context& io_context)
  {
    return pointer(new tcp_connection(io_context));
  }

  tcp::socket& socket()
  {
    return socket_;
  }

  void start()
  {
#define max_length 1024
      try
      {
        for (;;)
        {
          // read the data
          char data[max_length];
          memset(data, 0, sizeof(data));
          boost::system::error_code error;
          size_t length = socket_.read_some(boost::asio::buffer(data), error);
          if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
          else if (error)
            throw boost::system::system_error(error); // Some other error.

          printf("%s %zu\n", data, length);
          if(strcmp(data, "go") == 0){
              go_main();
          }

          // boost::asio::write(sock, boost::asio::buffer(data, length));

          // message_ = make_daytime_string();
        
          // boost::asio::async_write(socket_, boost::asio::buffer(message_),
          // boost::bind(&tcp_connection::handle_write, shared_from_this(),
          // boost::asio::placeholders::error,
          // boost::asio::placeholders::bytes_transferred));
        }
      }
      catch (std::exception& e)
      {
        std::cerr << "Exception in thread: " << e.what() << "\n";
      }
  }

private:
  tcp_connection(boost::asio::io_context& io_context)
    : socket_(io_context)
  {
  }

  void handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/)
  {
  }

  tcp::socket socket_;
  std::string message_;
};

class tcp_server
{
public:
  tcp_server(boost::asio::io_context& io_context)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), 7777))
  {
    start_accept();
  }

private:
  void start_accept()
  {
    tcp_connection::pointer new_connection =
      tcp_connection::create(acceptor_.get_executor().context());

    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&tcp_server::handle_accept, this, new_connection,
          boost::asio::placeholders::error));
  }

  void handle_accept(tcp_connection::pointer new_connection,
      const boost::system::error_code& error)
  {
    if (!error)
    {
      new_connection->start();
    }

    start_accept();
  }

  tcp::acceptor acceptor_;
};

int main()
{
  try
  {
    boost::asio::io_context io_context;
    tcp_server server(io_context);
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
