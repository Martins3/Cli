#ifndef HANDLER_H
#define HANDLER_H
#include <string>

class Handler{
public: 
  void add_time_point(std::string desc);
  void show_time_last();
  void insert_time_point(std::string event);
  void pop_time_point();

public:
    Handler(Handler const&)       = delete;
    void operator=(Handler const&)  = delete;
    static Handler& getInstance(){
        static Handler instance;
        return instance;
    }
private:
    Handler()=default;
};
#endif /* end of include guard: HANDLER_H */
