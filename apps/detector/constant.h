#ifndef CONSTANT_H
#define CONSTANT_H

#include <QString>


namespace detector {
  using SourceCaptureArray = std::map<int, QString>;
  const int kMaxCam = 10;
}

#endif // CONSTANT_H
