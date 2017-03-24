#ifndef VIDEOWRITER_H
#define VIDEOWRITER_H

#include <list>
#include <thread>
#include <mutex>
#include <atomic>

#include <QObject>
#include <opencv2/opencv.hpp>

#include "detectors/idetector.h"
#include "newframethread.h"


namespace detector {

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
  class VideoWriter : public QObject, public NewFrameThread
  {
      Q_OBJECT

    public:
      explicit VideoWriter(const std::string& path_file,
                           std::list< std::shared_ptr<IDetector> > detectors,
                           QObject* parent = 0);
      bool Start(const cv::Mat frame);
      void SetSize(const int& width, const int& height);
      void SetFps(const int& fps);

    private:
      std::string path_file_;
      int width_ {0};
      int height_ {0};
      int fps_ = {0};

      std::mutex mutex_write_;
      std::unique_ptr<cv::VideoWriter> video_writer_;

      DataList data_list_;

      std::atomic_int write_after_left_ {0};

      bool Begin();
      bool End();

    public slots:
      void slotFind(const IDetector* detector, const std::vector<cv::Rect> rectangles);
      void slotLose(const IDetector* detector);
  };

}

#endif // VIDEOWRITER_H
