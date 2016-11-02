#ifndef DETECTORFACE_H
#define DETECTORFACE_H

#include <opencv2/objdetect.hpp>

#include "detect/iobserver.h"


namespace detect {

  class DetectorFace : public IObserver
  {
    public:
      DetectorFace();
      std::vector<cv::Rect> Detect(const cv::Mat &frame);
      std::string Name();

    private:
      cv::CascadeClassifier face_cascade;
      cv::CascadeClassifier eyes_cascade;
  };

}

#endif // DETECTORFACE_H
