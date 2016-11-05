#include "core/core.h"

#include "detectors/idetector.h"


detect::IDetector::IDetector(const std::string& name, QObject* parent) :
  name_(name), QObject(parent)
{ }


detect::IDetector::~IDetector()
{
  work_result_.wait();
}


void detect::IDetector::NewFrame(const cv::Mat& frame)
{
  // Check previous work thread is finish job
  if (work_result_.valid()) {
    auto status = work_result_.wait_for(std::chrono::milliseconds(0));
    if (status != std::future_status::ready)
      return;
  }

  frame_ = frame;
  work_result_ = std::async(std::launch::async, &IDetector::Start, this);
}


std::string detect::IDetector::Name() const
{
  return name_;
}


bool detect::IDetector::Start()
{
  try
  {
    std::vector<cv::Rect> rects_find = Detect(frame_);
    if (rects_find.size() > 0) {
      LOG_INFO << name_ << " find count: " << rects_find.size();
      emit signalFind(this);
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
