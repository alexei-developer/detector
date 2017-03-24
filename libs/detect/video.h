#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <thread>
#include <atomic>

#include <opencv2/videoio.hpp>

#include "detectors/idetector.h"
#include "videowriter.h"


namespace detector {

  class VideoCapture
  {
    public:
      VideoCapture(const std::string& url, const std::list< std::shared_ptr<IDetector> >& detectors, const std::string path_save);
      VideoCapture(const int& usb_device, const std::list< std::shared_ptr<IDetector> >& detectors, const std::string path_save);
      ~VideoCapture();

      bool Start();
      bool Stop();

      void SetTextFile(const QString& path);

    private:
      cv::VideoCapture video_;
      std::thread work_;
      std::atomic_bool flag_stop_ {false};
      std::vector<std::shared_ptr<IDetector>> observers_;
      std::shared_ptr<VideoWriter> writer_;
      QString path_text_file_;

      void Capture();
  };

}

#endif // VIDEO_H
