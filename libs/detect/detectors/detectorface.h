#ifndef DETECTORFACE_H
#define DETECTORFACE_H

#include <opencv2/objdetect.hpp>

#include "detect/detectors/idetector.h"


namespace detector {

  class DetectorFace : public IDetector
  {
    public:
      DetectorFace();
      std::vector<cv::Rect> Detect(const cv::Mat &frame);
  };

}

#endif // DETECTORFACE_H
