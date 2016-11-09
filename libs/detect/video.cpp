#include "video.h"
#include "core/core.h"
#include "videowriter.h"


detect::VideoCapture::VideoCapture(const std::string& url)
{
  LOG_DEBUG << "Set IP device";
  if (!video_.open(url))
    throw std::runtime_error("Can not set IP device");
}


detect::VideoCapture::VideoCapture(const int& usb_device)
{
  LOG_DEBUG << "Set usb device";
  if (!video_.open(usb_device))
    throw std::runtime_error("Can not set usb device");
}


detect::VideoCapture::~VideoCapture()
{
  Stop();
  work_.join();
}


bool detect::VideoCapture::Start()
{
  try {
    LOG_INFO << "Start video surveliance...";
    flag_stop_ = false;

    if(!video_.isOpened()) {
      LOG_ERROR << "Device not open";
      return false;
    }

    work_ = std::thread(&VideoCapture::Capture, this);
  }
  catch (std::exception& e) {
    LOG_CRITICAL << "Error: " << e.what();
    return false;
  }

  LOG_INFO << "Start video surveliance is OK";
  return true;
}


bool detect::VideoCapture::Stop()
{
  LOG_INFO << "Request for stop video surveliance";
  flag_stop_ = true;
}


void detect::VideoCapture::AddDetector(IDetector* observer)
{
  LOG_INFO << "Add detector: " << observer->Name();
  observers_.push_back(observer);
}


void detect::VideoCapture::Capture()
{
  const int frame_width  =   video_.get(CV_CAP_PROP_FRAME_WIDTH);
  const int frame_height =   video_.get(CV_CAP_PROP_FRAME_HEIGHT);
  VideoWriter writer("/tmp", frame_width, frame_height, observers_);

  LOG_INFO << "Start capture";
  cv::Mat frame;

  while (!flag_stop_)
  {
    video_ >> frame;
    if(frame.empty())
      throw std::runtime_error("Can not read frame");

    for (IDetector* observer : observers_)
      observer->NewFrame(frame);

    writer.NewFrame(frame);
  }

  LOG_INFO << "Stop capture";
}
