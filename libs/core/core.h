#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <sstream>
#include <cstring>


#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)


class Log
{
  public:

    enum Level {
      DEBUG,
      INFO,
      ERR
    };

    Log(const Level& level);
    ~Log();

    template <typename T> Log &operator<<(T t)
    {
      stream_ << t << " "; return *this;
    }

    inline Log &operator<<(bool t);


  private:
    std::stringstream stream_;
    Level level_;
};


#define LOG_DEBUG    Log(Log::DEBUG)
#define LOG_INFO     Log(Log::INFO)
#define LOG_WARNING  Log(Log::ERR)
#define LOG_CRITICAL Log(Log::ERR)



template<typename T>
std::string ToString(const T& val) {
  std::stringstream s;
  s << val;
  return s.str();
}

#endif // CORE_H
