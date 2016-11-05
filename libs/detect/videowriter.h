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

      std::list<const IDetector*> detectors_;
      bool IsPresent(const IDetector* detector) const;

    public slots:
      void slotFind(const IDetector* detector);
      void slotLose(const IDetector* detector);
  };

}

#endif // VIDEOWRITER_H
