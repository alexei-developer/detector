#ifndef DETECTOR_H
#define DETECTOR_H

#include <QtCore>

#include "detect/video.h"


namespace detector {

  class Detector
  {
    public:
      Detector();
      bool Start();

    private:
      std::map<int, std::shared_ptr<VideoCapture>> video_captures_;
  };

}

#endif // DETECTOR_H
