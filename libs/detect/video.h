#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <thread>
#include <atomic>

#include <opencv2/videoio.hpp>

#include "detectors/idetector.h"
#include "videowriter.h"


namespace detect {

  class VideoCapture
  {
    public:
      VideoCapture(const std::string& url);
      VideoCapture(const int& usb_device);
      ~VideoCapture();

      bool Start();
      bool Stop();

      void AddDetector(IDetector* observer);
      void SetWriter(std::shared_ptr<VideoWriter> writer);

    private:
      cv::VideoCapture video_;
      std::thread work_;
      std::atomic_bool flag_stop_ {false};
      std::vector<IDetector*> observers_;
      std::shared_ptr<VideoWriter> writer_;

      void Capture();
  };

}

#endif // VIDEO_H
