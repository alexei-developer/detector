#include <QSettings>

#include "detector.h"

#include "config.h"
#include "core/core.h"
#include "detect/video.h"
#include "detect/detectors/detectorface.h"
#include "detect/detectors/detectormotion.h"

using namespace detector;

Detector::Detector()
{
  QSettings settings("setting.ini", QSettings::IniFormat);

  // General
  const QString path_save = settings.value("path_save", "").toString();

  try {
    // Detectors
    settings.beginGroup("Detectors");

    std::list< std::shared_ptr<IDetector> > detectors;
    if (settings.value("motion", false).toBool())
      detectors.push_back(std::make_shared<DetectorMotion>(0.1));

    if (settings.value("face", false).toBool())
      detectors.push_back(std::make_shared<DetectorFace>());
    settings.endGroup();

    // VideoCapture
    settings.beginGroup("Cams");
    const int kMaxCam = 10;
    for(int i_source = 0; i_source < kMaxCam; ++i_source) {
      const QString source_capture = settings.value("source_" + QString::number(i_source), "").toString();

      if (source_capture.isEmpty())
        continue;

      // Create dir if not exists
      const QString path_save_each_cam = path_save + "/cam_" + QString::number(i_source);
      QDir dir_eac_cam(path_save_each_cam);
      if ( ! dir_eac_cam.exists())
        dir_eac_cam.mkpath(path_save_each_cam);

      // Auto choose usb or url device
      bool is_device = false;
      int device = source_capture.toInt(&is_device);

      if (is_device)
        video_captures_[i_source] = std::make_shared<detector::VideoCapture>(device,
                                                                          detectors,
                                                                          path_save_each_cam.toStdString());
      else
        video_captures_[i_source] = std::make_shared<detector::VideoCapture>(source_capture.toStdString(),
                                                                 detectors,
                                                                 path_save_each_cam.toStdString());

      const QString text_capture = settings.value("textfile_" + QString::number(i_source), "").toString();
      if ( ! text_capture.isEmpty()) {
        video_captures_[i_source]->SetTextFile(text_capture);
      }
    }
    settings.endGroup();
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
