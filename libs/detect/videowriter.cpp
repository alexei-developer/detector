#include <algorithm>

#include "videowriter.h"
#include "core/core.h"


using namespace detect;


// === Data ===


bool Data::operator==(const Data& other)
{
  return detector == other.detector;
}


bool DataList::Add(const IDetector* detector, const std::vector<cv::Rect> rectangles)
{
  if (IsPresent(detector)) {
    Update(detector, rectangles);
    return false;
  }

  data_.push_back({detector, rectangles});
  return true;
}


bool DataList::Delete(const IDetector* detector)
{
  for(std::list<Data>::iterator it = data_.begin(); it != data_.end(); ++it)
  {
    if ((*it).detector == detector)
    {
      data_.remove(*it);
      return true;
    }
  }

  return false;
}


bool DataList::IsPresent(const IDetector* detector) const
{
  for(Data data : data_)
    if (data.detector == detector)
      return true;

  return false;
}


const int DataList::Size() const
{
  return data_.size();
}


const std::vector<cv::Rect> DataList::GetRectangles() const
{
  std::vector<cv::Rect> rect_all;

  for(const Data& data : data_)
    for(const cv::Rect& rect : data.rectangles)
      rect_all.push_back(rect);

  return rect_all;
}


bool DataList::Update(const IDetector* detector, const std::vector<cv::Rect> rectangles)
{
  for(std::list<Data>::iterator it = data_.begin(); it != data_.end(); ++it)
  {
    if ((*it).detector == detector)
    {
      (*it).rectangles = rectangles;
      return true;
    }
  }

  return false;
}

// ^^^ Data ^^^


const int ERR = -1;
const std::string FILENAME = "detector";


VideoWriter::VideoWriter(const std::string& path_file,
                         const int& width, const int& height,
                         std::vector<IDetector*> detectors,
                         QObject *parent) :
  path_file_(path_file), width_(width), height_(height), QObject(parent)
{
  for(IDetector* detector : detectors) {
    connect(detector, SIGNAL(signalFind(const IDetector*,std::vector<cv::Rect>)),
            this,     SLOT(slotFind(const IDetector*,std::vector<cv::Rect>)),
            Qt::DirectConnection);
    connect(detector, SIGNAL(signalLose(const IDetector*)),
            this,     SLOT(slotLose(const IDetector*)),
            Qt::DirectConnection);
  }
}


void VideoWriter::Write(const cv::Mat& frame)
{
  if (data_list_.Size() == 0)
    return;

  cv::Mat frame_with_rects = frame;
  for(const cv::Rect& rect : data_list_.GetRectangles())
    cv::rectangle(frame_with_rects, rect, cv::Scalar(0, 0, 255));

  std::lock_guard<std::mutex> lock(mutex_write_);
  if (video_writer_)
    if (video_writer_->isOpened()) {
      LOG_INFO << "WRITE";
      video_writer_->write(frame_with_rects);
    }
}


void VideoWriter::slotFind(const IDetector* detector, const std::vector<cv::Rect> rectangles)
{
  if ( ! data_list_.Add(detector, rectangles))
    return;

  if (data_list_.Size() == 1) {
    static int count_files = 0;
    LOG_INFO << "Begin write: " << count_files;

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
  if ( ! data_list_.IsPresent(detector))
    return;

  data_list_.Delete(detector);

  if (data_list_.Size() == 0) {
    LOG_INFO << "End write";
    std::lock_guard<std::mutex> lock(mutex_write_);
    video_writer_.reset();
  }
}
