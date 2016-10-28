#include "core.h"


#if (QT_VERSION < QT_VERSION_CHECK(5,5,0))
qInfo::~qInfo()
{
  std::cout << stream.str() << std::endl;
}


qInfo& qInfo::operator<<(bool t)
{
  stream << (t ? "True" : "False"); return *this;
}
#endif
