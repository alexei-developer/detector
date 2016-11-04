#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <thread>
#include <atomic>

#include <opencv2/videoio.hpp>

#include "iobserver.h"


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

    private:
      cv::VideoCapture video_;
      std::thread work_;
      std::atomic_bool flag_stop_ {false};
      std::vector<IDetector*> observers_;

      void Capture();
  };

}

#endif // VIDEO_H
