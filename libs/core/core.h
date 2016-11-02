#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <sstream>
#include <cstring>

#define GCC_VERSION (__GNUC__ * 10000 + \
                     __GNUC_MINOR__ * 100 + \
                     __GNUC_PATCHLEVEL__)

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#ifdef NDEBUG
#define LOG_PREFIX ""
#else
#define LOG_PREFIX "(" << __FILENAME__ << " | " << __FUNCTION__ << " | " << __LINE__ << ") "
#endif

namespace developer {

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


  #define LOG_DEBUG    developer::Log(developer::Log::DEBUG) << LOG_PREFIX
  #define LOG_INFO     developer::Log(developer::Log::INFO)  << LOG_PREFIX
  #define LOG_WARNING  developer::Log(developer::Log::ERR)   << LOG_PREFIX
  #define LOG_ERROR    developer::Log(developer::Log::ERR)   << LOG_PREFIX
  #define LOG_CRITICAL developer::Log(developer::Log::ERR)   << LOG_PREFIX



  template<typename T>
  std::string ToString(const T& val) {
    std::stringstream s;
    s << val;
    return s.str();
  }

}


#endif // CORE_H
