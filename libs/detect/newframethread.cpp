#include "newframethread.h"

using namespace detector;


NewFrameThread::NewFrameThread()
{
}


NewFrameThread::~NewFrameThread()
{
  work_result_.wait();
}


void NewFrameThread::NewFrame(const cv::Mat& frame)
{
  // Check previous work thread is finish job
  if (work_result_.valid()) {
    auto status = work_result_.wait_for(std::chrono::milliseconds(0));
    if (status != std::future_status::ready)
      return;
  }

  work_result_ = std::async(std::launch::async, &NewFrameThread::Start, this, frame);
}


