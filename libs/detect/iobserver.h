#ifndef IOBSERVER_H
#define IOBSERVER_H

#include <thread>
#include <atomic>

#include <opencv2/core.hpp>


namespace detect {

  class IObserver
  {
    public:
      IObserver(const std::string& name);
      void NewFrame(const cv::Mat& frame);

    protected:
      virtual std::vector<cv::Rect> Detect(const cv::Mat& frame) = 0;

    private:
      const std::string name_;
      std::thread work_;
      std::atomic_bool is_busy_ {false};

      void Start(const cv::Mat& frame);
  };
}

#endif // IOBSERVER_H
