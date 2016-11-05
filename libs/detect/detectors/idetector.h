#ifndef IOBSERVER_H
#define IOBSERVER_H

#include <thread>
#include <future>

#include <QObject>
#include <opencv2/core.hpp>


namespace detect {

  /**
   * @brief The IObserver is observer pattern for find specify object
   */
  class IDetector : public QObject
  {
      Q_OBJECT

    public:
      IDetector(const std::string& name, QObject* parent = 0);
      virtual ~IDetector();

      void NewFrame(const cv::Mat& frame);
      std::string Name() const;

    protected:
      virtual std::vector<cv::Rect> Detect(const cv::Mat& frame) = 0;

    private:
      const std::string name_;
      std::future<bool> work_result_;
      cv::Mat frame_;

      bool Start();

    signals:

      void signalFind(const IDetector* detector, const std::vector<cv::Rect> rectangles);
      void signalLose(const IDetector* detector);
  };

}

#endif // IOBSERVER_H
