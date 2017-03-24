#include <QFile>

#include "video.h"
#include "core/core.h"


detector::VideoCapture::VideoCapture(const std::string& url,
                                     const std::list< std::shared_ptr<IDetector> >& detectors,
                                     const std::string path_save)
{
  for(std::shared_ptr<IDetector> detector: detectors) {
    LOG_INFO << "Add detector: " << detector->Name();
    observers_.push_back(detector);
  }

  LOG_DEBUG << "Set IP device: " << url;
  if (!video_.open(url))
    throw std::runtime_error("Can not set IP device");

  writer_ = std::make_shared<VideoWriter>(path_save, detectors);
}


detector::VideoCapture::VideoCapture(const int& usb_device, const std::list< std::shared_ptr<IDetector> >& detectors, const std::string path_save)
{
  for(std::shared_ptr<IDetector> detector: detectors) {
    LOG_INFO << "Add detector: " << detector->Name();
    observers_.push_back(detector);
  }

  LOG_DEBUG << "Set usb device: " << usb_device;
  if (!video_.open(usb_device))
    throw std::runtime_error("Can not set usb device");

  writer_ = std::make_shared<VideoWriter>(path_save, detectors);
}


detector::VideoCapture::~VideoCapture()
{
  Stop();
  work_.join();
}


bool detector::VideoCapture::Start()
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


bool detector::VideoCapture::Stop()
{
  LOG_INFO << "Request for stop video surveliance";
  flag_stop_ = true;
}


void detector::VideoCapture::SetTextFile(const QString& path)
{
  path_text_file_ = path;
}


void detector::VideoCapture::Capture()
{
  LOG_INFO << "Start capture";
  cv::Mat frame;
  static std::string text_on_cam;

  while (!flag_stop_)
  {
    video_ >> frame;
    if(frame.empty())
      throw std::runtime_error("Can not read frame");

    QFile text_file(path_text_file_);
    if (text_file.open(QFile::ReadOnly))
      text_on_cam = text_file.readAll().toStdString();
    cv::putText(frame, text_on_cam, cv::Point(100, 100),
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(200,200,250), 2);

    for (std::shared_ptr<IDetector> observer : observers_)
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
