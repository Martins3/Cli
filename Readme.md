# Timestamp

## why we want to build this tools
The only only way to be sharp is time limitation.
![](./src/readme-1.jpg)

This is not simply a time tracker, it is a framework to resharp me.
No more pretending to work harp while watching youtube from time to time.


## How to use it
Record before you start it.

## TODO
1. create seperate files for different time
2. for every events, we can add explnation to it
3. use web to show time line
  1. it should be easy
  2. hard code it instead of reading a file
4. add thinking about what is currently do


## Watch Out
The auther of this repo is not familiar with boost and C++, you have anything advice about the code, issue it !

## what can it do
1. As a tomato clock
1. When boot, a daemon started, used for as clock
2. Statisc aciton and analyze it, day, week and month
3. Notification
4. Upload data to cloud and make a statistic of every user
5. Read process message


## Architecture
We rely on this boost library
https://www.boost.org/doc/libs/1_36_0/doc/html/boost_asio/tutorial/tuttimer1.html
Server layer and client layer use the TCP for connection.

1. User
2. Statistic
    1. uniform data type
    2. maybe use some python and something else.
3. Server
    1. for reading data

## with more conscious 

## we can generate the time line in the html format



## Techs
1. json
    1. install
    2. easy to anazyze data
2. make install

[getopts](https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html)
[now](https://en.cppreference.com/w/cpp/chrono/system_clock/now)
[make install](https://robots.thoughtbot.com/the-magic-behind-configure-make-make-install)
[notifcation](https://askubuntu.com/questions/730050/how-to-use-notify-send-with-c)
[daemon](https://github.com/jirihnidek/daemon/tree/d629c1fb7f395ced63fbee791ae3319c875c459f/src)
[asio](https://www.boost.org/doc/libs/1_68_0/doc/html/boost_asio.html)
[check port in use](https://stackoverflow.com/questions/33358321/using-c-and-boost-or-not-to-check-if-a-specific-port-is-being-used)

https://stackoverflow.com/questions/1657883/variable-number-of-arguments-in-c
## install 
prequite

