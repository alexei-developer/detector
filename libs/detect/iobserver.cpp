#include "core/core.h"

#include "iobserver.h"


detect::IObserver::IObserver(const std::string& name) :
  name_(name)
{
}


void detect::IObserver::NewFrame(const cv::Mat& frame)
{
  if (is_busy_)
    return;
  LOG_INFO << "Get new frame for detect: " << name_;

  work_ = std::thread(&IObserver::Start, this, std::ref(frame));
}


void detect::IObserver::Start(const cv::Mat& frame)
{
  is_busy_ = true;
  LOG_DEBUG << "Begin find " << name_;

  try
  {
    std::vector<cv::Rect> rects_find = Detect(frame);
    LOG_INFO << name_ << " find count: " << rects_find.size();
  }
  catch (std::exception& e) {
    LOG_CRITICAL << e.what();
  }

  is_busy_ = false;
  LOG_DEBUG << "End find " << name_;
}
