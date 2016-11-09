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
const int WRITE_AFTER = 125; // 5 seconds


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


bool VideoWriter::Start(const cv::Mat frame)
{
  static int count_files = 0;

  // Write running
  std::lock_guard<std::mutex> lock(mutex_write_);
  if (!video_writer_) {
    if (Begin()) {
      ++count_files;

      LOG_INFO << "Begin write: " << count_files;
      video_writer_.reset(
            new cv::VideoWriter(path_file_ + "/" + FILENAME + std::to_string(count_files) + ".avi",
                         CV_FOURCC('M','J','P','G'),
                         25,
                         cv::Size(width_, height_))
            );
    }
    return false;
  }


  if (End()) {
    LOG_INFO << "End write: " << count_files;
    video_writer_.reset();
    return false;
  }

  if (!video_writer_->isOpened())
    return false;

  // Write to file
  cv::Mat frame_with_rects = frame;
  for(const cv::Rect& rect : data_list_.GetRectangles())
    cv::rectangle(frame_with_rects, rect, cv::Scalar(0, 0, 255));

  video_writer_->write(frame_with_rects);
  return true;
}


bool VideoWriter::Begin()
{
  return (!video_writer_ && data_list_.Size() == 1);
}


bool VideoWriter::End()
{
  if (write_after_left_ > 0) {
    --write_after_left_;
    return false;
  }

  if (data_list_.Size() != 0)
    return false;

  return true;
}


void VideoWriter::slotFind(const IDetector* detector, const std::vector<cv::Rect> rectangles)
{
  write_after_left_ = WRITE_AFTER;

  if ( ! data_list_.Add(detector, rectangles))
    return;
}


void VideoWriter::slotLose(const IDetector* detector)
{
  if ( ! data_list_.IsPresent(detector))
    return;

  data_list_.Delete(detector);
}
