#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "detectormotion.h"
#include "core/core.h"


using namespace detector;


DetectorMotion::DetectorMotion(const double& percent) :
  percent_(percent), IDetector("Motion")
{ }


std::vector<cv::Rect> DetectorMotion::Detect(const cv::Mat& frame)
{
  std::vector<cv::Rect> rect;

  if (frame_previous_.empty()) {
    frame.copyTo(frame_previous_);
    return rect;
  }

  cv::absdiff(frame, frame_previous_, frame_diff_);
  cv::cvtColor(frame_diff_, frame_mask_, CV_BGR2GRAY);

  float threashold = 30.0;
  frame_motion_ = frame_mask_ < threashold;

  int count_motion = 0;
  for(int i = 0; i < frame_motion_.rows; ++i)
    for(int j = 0; j < frame_motion_.cols; ++j)
      if (frame_motion_.at<uchar>(i,j) == 0)
        ++count_motion;

  double percent = count_motion * 100.0 / double(frame_motion_.rows * frame_motion_.cols);
  if (percent > percent_)
    rect.push_back(cv::Rect(1, 1, 1, 1));

  frame.copyTo(frame_previous_);
  return rect;
}
