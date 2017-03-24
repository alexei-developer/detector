#ifndef NEWFRAMETHREAD_H
#define NEWFRAMETHREAD_H

#include <thread>
#include <future>

#include <opencv2/core.hpp>


namespace detector {

  class NewFrameThread
  {
    public:
      NewFrameThread();
      virtual ~NewFrameThread();

      void NewFrame(const cv::Mat& frame);

    protected:
      virtual bool Start(const cv::Mat frame) = 0;

    private:
      std::future<bool> work_result_;
  };

}

#endif // NEWFRAMETHREAD_H
