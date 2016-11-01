#include <exception>
#include <iostream>
#include <thread>

#include <QCoreApplication>
#include <QStringList>

#include "config.h"
#include "core/core.h"
#include "detect/detect.h"


int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  LOG_INFO << "Detector version:" << DETECTOR_VERSION;
  LOG_INFO << "Qt version:" << qVersion();

  LOG_DEBUG << "Arguments:" << a.arguments().join(" ").toStdString();

  try {
    if (!init())
      return 1;

    DetectFace(PATH_RESOURCES "/images/faces.jpg");
  }
  catch (DetectNotFound& e) {
    LOG_INFO << e.what();
    return 0;
  }
  catch (std::exception& e) {
    LOG_CRITICAL << "Error: " << e.what();
    return 1;
  }
  catch (...) {
    LOG_CRITICAL << "Undefined error";
    return 2;
  }

  return 0;
}
