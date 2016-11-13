#include "video.h"
#include "core/core.h"


detect::VideoCapture::VideoCapture(const std::string& url)
{
  LOG_DEBUG << "Set IP device: " << url;
  if (!video_.open(url))
    throw std::runtime_error("Can not set IP device");
}


detect::VideoCapture::VideoCapture(const int& usb_device)
{
  LOG_DEBUG << "Set usb device: " << usb_device;
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


void detect::VideoCapture::SetWriter(std::shared_ptr<detect::VideoWriter> writer)
{
  writer_ = writer;
}


void detect::VideoCapture::Capture()
{
  LOG_INFO << "Start capture";
  cv::Mat frame;

  while (!flag_stop_)
  {
    video_ >> frame;
    if(frame.empty())
      throw std::runtime_error("Can not read frame");

    for (IDetector* observer : observers_)
      observer->NewFrame(frame);

    if (writer_) {
      writer_->SetSize(
            video_.get(CV_CAP_PROP_FRAME_WIDTH),
            video_.get(CV_CAP_PROP_FRAME_HEIGHT)
            );
      writer_->SetFps(
            video_.get(CV_CAP_PROP_FPS)
            );
      writer_->NewFrame(frame);
    }
  }

  LOG_INFO << "Stop capture";
}
