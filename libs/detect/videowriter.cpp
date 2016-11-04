#include <algorithm>

#include "videowriter.h"
#include "core/core.h"


using namespace detect;


const int ERR = -1;
const std::string FILENAME = "detector";


VideoWriter::VideoWriter(const std::string& path_file,
                         const int& width, const int& height,
                         std::vector<IDetector*> detectors,
                         QObject *parent) :
  path_file_(path_file), width_(width), height_(height), QObject(parent)
{
  for(IDetector* detector : detectors) {
    connect(detector, SIGNAL(signalFind(const IDetector*)), this, SLOT(slotFind(const IDetector*)), Qt::DirectConnection);
    connect(detector, SIGNAL(signalLose(const IDetector*)), this, SLOT(slotLose(const IDetector*)), Qt::DirectConnection);
  }
}


void VideoWriter::Write(const cv::Mat& frame)
{
  if (detectors_.size() == 0)
    return;

  std::lock_guard<std::mutex> lock(mutex_write_);
  if (video_writer_)
    if (video_writer_->isOpened()) {
      LOG_INFO << "WRITE";
      video_writer_->write(frame);
    }
}


bool VideoWriter::IsPresent(const IDetector* detector) const
{
   return std::find(detectors_.begin(), detectors_.end(), detector) != detectors_.end();
}


void VideoWriter::slotFind(const IDetector* detector)
{
  if (IsPresent(detector))
    return;

  detectors_.push_back(detector);

  if (detectors_.size() == 1) {
    LOG_INFO << "Begin write";
    static int count_files = 0;

    std::lock_guard<std::mutex> lock(mutex_write_);

    video_writer_.reset(
          new cv::VideoWriter(path_file_ + "/" + FILENAME + std::to_string(++count_files) + ".avi",
                       CV_FOURCC('M','J','P','G'),
                       25,
                       cv::Size(width_, height_))
          );
  }
}


void VideoWriter::slotLose(const IDetector* detector)
{
  if ( ! IsPresent(detector))
    return;

  detectors_.remove(detector);

  if (detectors_.size() == 0) {
    LOG_INFO << "End write";
    std::lock_guard<std::mutex> lock(mutex_write_);
    video_writer_.reset();
  }
}
