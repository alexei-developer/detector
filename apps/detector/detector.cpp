#include "detector.h"

#include "config.h"
#include "core/core.h"
#include "detect/video.h"
#include "detect/detectors/detectorface.h"


Detector::Detector(const QString& url)
{
  try {
    bool is_device = false;
    int device = url.toInt(&is_device);

    if (is_device)
      video = std::make_shared<detect::VideoCapture>(device);
    else
      video = std::make_shared<detect::VideoCapture>(url.toStdString());

    video->AddDetector(&detectorFace);
  }
  catch (const std::exception& e) {
    LOG_CRITICAL << "Error: " << e.what();
    QCoreApplication::exit(EXIT_CODE::ERR);
  }
  catch (...) {
    LOG_CRITICAL << "Undefined error";
    QCoreApplication::exit(EXIT_CODE::UNKNOW_ERR);
  }
}


bool Detector::Start()
{
  return video->Start();
}
