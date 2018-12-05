//
// daemon.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <ctime>
#include <iostream>
#include <syslog.h>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;


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
    message_ = make_daytime_string();

    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        boost::bind(&tcp_connection::handle_write, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
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
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), 6666))
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
    boost::asio::io_service io_service;

    // Initialise the server before becoming a daemon. If the process is
    // started from a shell, this means any errors will be reported back to the
    // user.
    tcp_server server(io_service);

    // Register signal handlers so that the daemon may be shut down. You may
    // also want to register for other signals, such as SIGHUP to trigger a
    // re-read of a configuration file.
    boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);
    signals.async_wait(
        boost::bind(&boost::asio::io_service::stop, &io_service));

    // Inform the io_service that we are about to become a daemon. The
    // io_service cleans up any internal resources, such as threads, that may
    // interfere with forking.
    io_service.notify_fork(boost::asio::io_service::fork_prepare);

    // Fork the process and have the parent exit. If the process was started
    // from a shell, this returns control to the user. Forking a new process is
    // also a prerequisite for the subsequent call to setsid().
    if (pid_t pid = fork())
    {
      if (pid > 0)
      {
        // We're in the parent process and need to exit.
        //
        // When the exit() function is used, the program terminates without
        // invoking local variables' destructors. Only global variables are
        // destroyed. As the io_service object is a local variable, this means
        // we do not have to call:
        //
        //   io_service.notify_fork(boost::asio::io_service::fork_parent);
        //
        // However, this line should be added before each call to exit() if
        // using a global io_service object. An additional call:
        //
        //   io_service.notify_fork(boost::asio::io_service::fork_prepare);
        //
        // should also precede the second fork().
        exit(0);
      }
      else
      {
        std::cout << "First fork failed" << std::endl;
        syslog(LOG_ERR | LOG_USER, "First fork failed: %m");
        return 1;
      }
    }

    // Make the process a new session leader. This detaches it from the
    // terminal.
    setsid();

    // A process inherits its working directory from its parent. This could be
    // on a mounted filesystem, which means that the running daemon would
    // prevent this filesystem from being unmounted. Changing to the root
    // directory avoids this problem.
    chdir("/");

    // The file mode creation mask is also inherited from the parent process.
    // We don't want to restrict the permissions on files created by the
    // daemon, so the mask is cleared.
    umask(0);

    // A second fork ensures the process cannot acquire a controlling terminal.
    if (pid_t pid = fork())
    {
      if (pid > 0)
      {
        exit(0);
      }
      else
      {
        std::cout << "Second fork failed" << std::endl;
        syslog(LOG_ERR | LOG_USER, "Second fork failed: %m");
        return 1;
      }
    }

    // Close the standard streams. This decouples the daemon from the terminal
    // that started it.
    close(0);
    close(1);
    close(2);

    // We don't want the daemon to have any standard input.
    if (open("/dev/null", O_RDONLY) < 0)
    {
      std::cout << "Unable to open /dev/null" << std::endl;
      syslog(LOG_ERR | LOG_USER, "Unable to open /dev/null: %m");
      return 1;
    }

    // Send standard output to a log file.
    const char* output = "/tmp/asio.daemon.out";
    const int flags = O_WRONLY | O_CREAT | O_APPEND;
    const mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    if (open(output, flags, mode) < 0)
    {
      syslog(LOG_ERR | LOG_USER, "Unable to open output file %s: %m", output);
      return 1;
    }

    // Also send standard error to the same log file.
    if (dup(1) < 0)
    {
      syslog(LOG_ERR | LOG_USER, "Unable to dup output descriptor: %m");
      return 1;
    }

    // Inform the io_service that we have finished becoming a daemon. The
    // io_service uses this opportunity to create any internal file descriptors
    // that need to be private to the new process.
    io_service.notify_fork(boost::asio::io_service::fork_child);

    // The io_service can now be used normally.
    syslog(LOG_INFO | LOG_USER, "Daemon started");
    std::cout << "Daemon started" << std::endl;
    io_service.run();
    syslog(LOG_INFO | LOG_USER, "Daemon stopped");
    std::cout << "Daemon Stopped" << std::endl;
  }
  catch (std::exception& e)
  {
    syslog(LOG_ERR | LOG_USER, "Exception: %s", e.what());
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}
