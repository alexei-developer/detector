#include "core/core.h"

#include "iobserver.h"


detect::IObserver::IObserver(const std::string& name) :
  name_(name)
{
}


detect::IObserver::~IObserver()
{
  work_result_.wait();
}


void detect::IObserver::NewFrame(const cv::Mat& frame)
{
  // Check previous work thread is finish job
  if (work_result_.valid()) {
    auto status = work_result_.wait_for(std::chrono::milliseconds(0));
    if (status != std::future_status::ready)
      return;
  }

  LOG_INFO << "Get new frame for detect: " << name_;

  frame_ = frame;
  work_result_ = std::async(std::launch::async, &IObserver::Start, this);
}


std::string detect::IObserver::Name() const
{
  return name_;
}


bool detect::IObserver::Start()
{
  LOG_DEBUG << "Begin find: " << name_;

  try
  {
    std::vector<cv::Rect> rects_find = Detect(frame_);
    LOG_INFO << name_ << " find count: " << rects_find.size();
  }
  catch (std::exception& e) {
    LOG_CRITICAL << e.what();
    return false;
  }

  LOG_DEBUG << "End find: " << name_;
  return true;
}
