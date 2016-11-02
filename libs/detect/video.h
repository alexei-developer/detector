#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <thread>
#include <atomic>

#include <opencv2/videoio.hpp>

#include "iobserver.h"


namespace detect {

  class Video
  {
    public:
      Video(const std::string& url);
      Video(const int& usb_device);
      ~Video();

      bool Start();
      bool Stop();

      void AddDetector(IObserver* observer);

    private:
      cv::VideoCapture video_;
      std::thread work_;
      std::atomic_bool flag_stop_ {false};
      std::vector<IObserver*> observers_;

      void Capture();
  };

}

#endif // VIDEO_H
