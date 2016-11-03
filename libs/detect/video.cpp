#include "core/core.h"

#include "video.h"


detect::Video::Video(const std::string& url)
{
  LOG_DEBUG << "Set IP device";
  if (!video_.open(url))
    throw std::runtime_error("Can not set IP device");
}


detect::Video::Video(const int& usb_device)
{
  LOG_DEBUG << "Set usb device";
  if (!video_.open(usb_device))
    throw std::runtime_error("Can not set usb device");
}


detect::Video::~Video()
{
  Stop();
  work_.join();
}


bool detect::Video::Start()
{
  try {
    LOG_INFO << "Start video surveliance...";
    flag_stop_ = false;

    if(!video_.isOpened()) {
      LOG_ERROR << "Device not open";
      return false;
    }

    work_ = std::thread(&Video::Capture, this);
  }
  catch (std::exception& e) {
    LOG_CRITICAL << "Error: " << e.what();
    return false;
  }

  LOG_INFO << "Start video surveliance is OK";
  return true;
}


bool detect::Video::Stop()
{
  LOG_INFO << "Request for stop video surveliance";
  flag_stop_ = true;
}


void detect::Video::AddDetector(IObserver* observer)
{
  LOG_INFO << "Add detector: " << observer->Name();
  observers_.push_back(observer);
}


void detect::Video::Capture()
{
  LOG_INFO << "Start capture";
  cv::Mat frame;

  while (!flag_stop_)
  {
    video_ >> frame;
    if(frame.empty())
      throw std::runtime_error("Can not read frame");

    LOG_INFO << "Good read frame";

    for (IObserver* observer : observers_)
      observer->NewFrame(frame);

//      DetectFace(image);
//      detectPedestrian(image);
//      cv::imshow("Sample", image);
//      if(cv::waitKey(10) >= 0)
//        break;
  }

  LOG_INFO << "Stop capture";
}
