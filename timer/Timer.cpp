#include "Server.hpp"
#include "Timer.hpp"
#include <syslog.h>

/**
int main(){
  try{
    boost::asio::io_context io_context;
    tcp_server server(io_context);
    io_context.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
*/

#include <boost/asio.hpp>
#include <cstdlib>
#include <cstring>
#include <iostream>

using boost::asio::ip::tcp;

void Timer::send_message(const std::string msg) {
  const char * request = msg.c_str();
  try {
    boost::asio::io_context io_context;

    tcp::socket s(io_context);
    tcp::resolver resolver(io_context);
    boost::asio::connect(s, resolver.resolve("127.0.0.1", "7777"));

    size_t request_length = std::strlen(request);
    boost::asio::write(s, boost::asio::buffer(request, request_length));
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
}

bool Timer::is_daemon_alive() {
  const int port = 7777;
  using namespace boost::asio;
  using ip::tcp;

  io_service svc;
  tcp::acceptor a(svc);

  boost::system::error_code ec;
  a.open(tcp::v4(), ec) || a.bind({tcp::v4(), port}, ec);

  return ec == error::address_in_use;
}

void Timer::start_daemon() {
  if (is_daemon_alive()) {
    std::cout << "Server alive" << std::endl;
    return;
  }

  using boost::asio::ip::tcp;
  try {
    boost::asio::io_service io_service;
    boost::asio::io_service timer_io_service;

    // Initialise the server before becoming a daemon. If the process is
    // started from a shell, this means any errors will be reported back to the
    // user.
    tcp_server server(io_service, timer_io_service);

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
    if (pid_t pid = fork()) {
      if (pid > 0) {
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
      } else {
        std::cout << "First fork failed" << std::endl;
        syslog(LOG_ERR | LOG_USER, "First fork failed: %m");
        exit(1);
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
    if (pid_t pid = fork()) {
      if (pid > 0) {
        exit(0);
      } else {
        std::cout << "Second fork failed" << std::endl;
        syslog(LOG_ERR | LOG_USER, "Second fork failed: %m");
        exit(1);
      }
    }

    // Close the standard streams. This decouples the daemon from the terminal
    // that started it.
    close(0);
    close(1);
    close(2);

    // We don't want the daemon to have any standard input.
    if (open("/dev/null", O_RDONLY) < 0) {
      std::cout << "Unable to open /dev/null" << std::endl;
      syslog(LOG_ERR | LOG_USER, "Unable to open /dev/null: %m");
      exit(1);
    }

    // Send standard output to a log file.
    const char *output = "/tmp/hubachelar.timer.out";
    const int flags = O_WRONLY | O_CREAT | O_APPEND;
    const mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    if (open(output, flags, mode) < 0) {
      syslog(LOG_ERR | LOG_USER, "Unable to open output file %s: %m", output);
      exit(1);
    }

    // Also send standard error to the same log file.
    if (dup(1) < 0) {
      syslog(LOG_ERR | LOG_USER, "Unable to dup output descriptor: %m");
      exit(1);
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
  } catch (std::exception &e) {
    syslog(LOG_ERR | LOG_USER, "Exception: %s", e.what());
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}

void Timer::start() {}

void Timer::cancel() {}

void Timer::suspend() {}

bool Timer::is_timing() {
  // static message use file to connect is ok.
  return false;
}

int Timer::seconds_left() {
  // with the help of file io
  return 0;
}

void Timer::start_server(){
    boost::asio::io_service io_service;
    boost::asio::io_service timer_io_service;
    tcp_server server(io_service, timer_io_service);
    io_service.run();
}
