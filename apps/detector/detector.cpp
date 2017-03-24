#include "detector.h"

#include "config.h"
#include "core/core.h"
#include "detect/video.h"
#include "detect/detectors/detectorface.h"
#include "detect/detectors/detectormotion.h"

using namespace detector;

Detector::Detector(SourceCaptureArray capture_array, const QString& pathSave,
                   const bool& detectMotion, const bool& detectFace)
{
  try {
    // Detectors
    std::list< std::shared_ptr<IDetector> > detectors;
    if (detectMotion)
      detectors.push_back(std::make_shared<DetectorMotion>(0.1));

    if (detectFace)
      detectors.push_back(std::make_shared<DetectorFace>());

    // VideoCapture
    for(int i_cam = 0; i_cam < kMaxCam; ++i_cam) {
      if (capture_array[i_cam].isEmpty())
        continue;

      // Create dir if not exists
      const QString path_save_each_cam = pathSave + "/cam_" + QString::number(i_cam);
      QDir dir_eac_cam(path_save_each_cam);
      if ( ! dir_eac_cam.exists())
        dir_eac_cam.mkpath(path_save_each_cam);

      // Auto choose usb or url device
      bool is_device = false;
      int device = capture_array[i_cam].toInt(&is_device);

      if (is_device)
        video_captures_[i_cam] = std::make_shared<detector::VideoCapture>(device,
                                                                          detectors,
                                                                          path_save_each_cam.toStdString());
      else
        video_captures_[i_cam] = std::make_shared<detector::VideoCapture>(capture_array[i_cam].toStdString(),
                                                                 detectors,
                                                                 path_save_each_cam.toStdString());
    }
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
  bool start_all_successfull = true;
  for(std::pair<int, std::shared_ptr<VideoCapture>> video_capture : video_captures_) {
    if ( ! video_capture.second->Start())
      start_all_successfull = false;
  }

  return start_all_successfull;
}
