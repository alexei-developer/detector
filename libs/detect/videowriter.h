#ifndef VIDEOWRITER_H
#define VIDEOWRITER_H

#include <list>
#include <thread>
#include <mutex>
#include <atomic>

#include <QObject>
#include <opencv2/opencv.hpp>

#include "detectors/idetector.h"


namespace detect {

  struct Data {
      const IDetector* detector;
      std::vector<cv::Rect> rectangles;

      bool operator==(const Data& other);
  };


  class DataList {

    public:
      bool Add(const IDetector* detector, const std::vector<cv::Rect> rectangles);
      bool Delete(const IDetector* detector);
      bool IsPresent(const IDetector* detector) const;
      const int Size() const;

      const std::vector<cv::Rect> GetRectangles() const;

    private:
      std::list<Data> data_;
      bool Update(const IDetector* detector, const std::vector<cv::Rect> rectangles);
  };


  /**
   * @brief VideoWriter write video for if find object
   */
  class VideoWriter : public QObject
  {
      Q_OBJECT

    public:
      explicit VideoWriter(const std::string& path_file,
                           const int& width,
                           const int& height,
                           std::vector<IDetector*> detectors,
                           QObject* parent = 0);
      void Write(const cv::Mat& frame);

    private:
      const std::string path_file_;
      const int width_;
      const int height_;

      std::mutex mutex_write_;
      std::unique_ptr<cv::VideoWriter> video_writer_;

      DataList data_list_;

    public slots:
      void slotFind(const IDetector* detector, const std::vector<cv::Rect> rectangles);
      void slotLose(const IDetector* detector);
  };

}

#endif // VIDEOWRITER_H
