#ifndef DETECTORMOTION_H
#define DETECTORMOTION_H

#include "detect/detectors/idetector.h"


namespace detect {

  class DetectorMotion: public IDetector
  {
    public:
      DetectorMotion(const double& percent);
      std::vector<cv::Rect> Detect(const cv::Mat &frame);

    private:
      const double percent_;

      cv::Mat frame_previous_;
      cv::Mat frame_diff_;
      cv::Mat frame_mask_;

      cv::Mat frame_motion_;
  };

}

#endif // DETECTORMOTION_H
