#include "core/core.h"

#include "detectors/idetector.h"


using namespace detector;


IDetector::IDetector(const std::string& name, QObject* parent) :
  QObject(parent), NewFrameThread(), name_(name)
{
  LOG_INFO << "Create detector: " << name_;
}


std::string IDetector::Name() const
{
  return name_;
}


bool IDetector::Start(const cv::Mat frame)
{
  try
  {
    std::vector<cv::Rect> rects_find = Detect(frame);
    if (rects_find.size() > 0) {
      LOG_INFO << name_ << " find count: " << rects_find.size();
      emit signalFind(this, rects_find);
    }
    else {
      emit signalLose(this);
    }
  }
  catch (std::exception& e) {
    LOG_CRITICAL << e.what();
    return false;
  }

  return true;
}
