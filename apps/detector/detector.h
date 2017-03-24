#ifndef DETECTOR_H
#define DETECTOR_H

#include <QtCore>

#include "constant.h"
#include "detect/video.h"


namespace detector {

  class Detector
  {
    public:
      Detector(SourceCaptureArray capture_array, const QString& pathSave = "",
               const bool& detectMotion = false, const bool& detectFace = false);
      bool Start();

    private:
      std::map<int, std::shared_ptr<VideoCapture>> video_captures_;
  };

}

#endif // DETECTOR_H
