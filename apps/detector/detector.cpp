#include "detector.h"

#include "config.h"
#include "core/core.h"
#include "detect/video.h"
#include "detect/detectors/detectorface.h"
#include "detect/detectors/detectormotion.h"

using namespace detect;

Detector::Detector(const QString& url, const QString& pathSave,
                   const bool& detectMotion, const bool& detectFace)
{
  try {

    // VideoCapture
    bool is_device = false;
    int device = url.toInt(&is_device);

    if (is_device)
      video_ = std::make_shared<detect::VideoCapture>(device);
    else
      video_ = std::make_shared<detect::VideoCapture>(url.toStdString());

    // Detectors
    if (detectMotion)
      detectors_.push_back(std::make_shared<DetectorMotion>(0.1));
    if (detectFace)
      detectors_.push_back(std::make_shared<DetectorFace>());
    for (std::shared_ptr<IDetector> detector : detectors_)
      video_->AddDetector(detector.get());

    // VideoWriter
    writer_ = std::make_shared<VideoWriter>(pathSave.toStdString(), detectors_);
    video_->SetWriter(writer_);
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
  return video_->Start();
}
