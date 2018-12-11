#ifndef SERVER_H
#define SERVER_H
#include "Winder.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <ctime>
#include <iostream>
#include <string>

class tcp_connection : public boost::enable_shared_from_this<tcp_connection> {
public:
  typedef boost::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_context &io_context) {
    return pointer(new tcp_connection(io_context));
  }

  boost::asio::ip::tcp::socket &socket() { return socket_; }

  std::vector<std::string> start() {
    std::vector<std::string> msg;
    try {
      for (;;) {
        // read the data
        char data[1024];
        memset(data, 0, sizeof(data));
        boost::system::error_code error;
        socket_.read_some(boost::asio::buffer(data), error);
        if (error == boost::asio::error::eof) {
          std::cout << "Connection closed cleanly by peer" << std::endl;
          break;
        } else if (error)
          throw boost::system::system_error(error); // Some other error.
        msg.emplace_back(data);
      }
    } catch (std::exception &e) {
      std::cerr << "Exception in thread: " << e.what() << "\n";
    }
    return msg;
  }

private:
  tcp_connection(boost::asio::io_context &io_context) : socket_(io_context) {}

  void handle_write(const boost::system::error_code & /*error*/,
                    size_t /*bytes_transferred*/) {}

  boost::asio::ip::tcp::socket socket_;
  std::string message_;
};

class tcp_server {
public:
  tcp_server(boost::asio::io_context &io_context, boost::asio::io_context &timer_io_context)
      : acceptor_(io_context, boost::asio::ip::tcp::endpoint(
                                  boost::asio::ip::tcp::v4(), 7777)), p(timer_io_context) {
    start_accept();
      // start timer's io_context in the background
      // if the timer doesn't stopped correctly, segment fault is waiting for you
      boost::thread(boost::bind(&boost::asio::io_context::run, &timer_io_context)).detach();
  }

private:
  void start_accept() {
    tcp_connection::pointer new_connection =
        tcp_connection::create(acceptor_.get_executor().context());

    acceptor_.async_accept(new_connection->socket(),
                           boost::bind(&tcp_server::handle_accept, this,
                                       new_connection,
                                       boost::asio::placeholders::error));
  }

  void handle_accept(tcp_connection::pointer new_connection,
                     const boost::system::error_code &error) {
    if (!error) {
      std::vector<std::string> msg = new_connection->start();
      for(std::string & str : msg){
        std::cout << str << std::endl;
      }
      // only check the last one
      // just send notification instead
      p.set(3);
    }

    start_accept();
  }

  boost::asio::ip::tcp::acceptor acceptor_;
  Winder p;
};

#endif /* end of include guard: SERVER_H */
