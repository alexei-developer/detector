#ifndef IOBSERVER_H
#define IOBSERVER_H

#include <thread>
#include <future>

#include <opencv2/core.hpp>


namespace detect {

  class IObserver
  {
    public:
      IObserver(const std::string& name);
      virtual ~IObserver();

      void NewFrame(const cv::Mat& frame);
      std::string Name() const;

    protected:
      virtual std::vector<cv::Rect> Detect(const cv::Mat& frame) = 0;

    private:
      const std::string name_;
      std::future<bool> work_result_;
      cv::Mat frame_;

      bool Start();
  };

}

#endif // IOBSERVER_H
