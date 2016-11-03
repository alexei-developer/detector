#include <exception>
#include <iostream>
#include <thread>

#include <QCoreApplication>
#include <QStringList>

#include "config.h"
#include "core/core.h"
#include "detect/video.h"
#include "detect/detectors/detectorface.h"


int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  LOG_INFO << "Detector version:" << DETECTOR_VERSION;
  LOG_INFO << "Qt version:" << qVersion();

  LOG_DEBUG << "Arguments:" << a.arguments().join(" ").toStdString();

  detect::DetectorFace detectorFace;
  try {
    detect::Video video(0);
    video.AddDetector(&detectorFace);
    video.Start();

    LOG_INFO << "Press <enter> key for finish...";
    std::cin.get();
  }
  catch (const std::exception& e) {
    LOG_CRITICAL << "Error: " << e.what();
    return 1;
  }
  catch (...) {
    LOG_CRITICAL << "Undefined error";
    return 2;
  }

  return 0;
}
