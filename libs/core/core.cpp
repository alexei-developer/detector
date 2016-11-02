#include "core.h"

using namespace developer;


Log::Log(const Log::Level& level) : level_(level)
{ }


Log::~Log()
{
  switch (level_) {
    case DEBUG:
#ifndef NDEBUG
      std::cout << stream_.str() << std::endl;
#endif
      break;

    case INFO:
      std::cout << stream_.str() << std::endl;
      break;

    case ERR:
      std::cout << stream_.str() << std::endl;
      break;

    default:
      break;
  }

}


Log& Log::operator<<(bool t)
{
  stream_ << (t ? "True" : "False") << " "; return *this;
}
