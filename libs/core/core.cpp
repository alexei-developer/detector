#include "core.h"

#ifdef NDEBUG
#define LOG_PREFIX ""
#else
#define LOG_PREFIX "(" << __FILENAME__ << " | " << __FUNCTION__ << " | " << __LINE__ << ") "
#endif


Log::Log(const Log::Level& level) : level_(level)
{ }


Log::~Log()
{
  switch (level_) {
    case DEBUG:
#ifndef NDEBUG
      std::cout << LOG_PREFIX << stream_.str() << std::endl;
#endif
      break;

    case INFO:
      std::cout << LOG_PREFIX << stream_.str() << std::endl;
      break;

    case ERR:
      std::cout << LOG_PREFIX << stream_.str() << std::endl;
      break;

    default:
      break;
  }

}


Log& Log::operator<<(bool t)
{
  stream_ << (t ? "True" : "False") << " "; return *this;
}
