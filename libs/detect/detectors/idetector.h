#ifndef IOBSERVER_H
#define IOBSERVER_H

#include <QObject>

#include "detect/newframethread.h"


namespace detect {

  /**
   * @brief The IObserver is observer pattern for find specify object
   */
  class IDetector : public QObject, public NewFrameThread
  {
      Q_OBJECT

    public:
      IDetector(const std::string& name, QObject* parent = 0);
      std::string Name() const;

    protected:
      virtual std::vector<cv::Rect> Detect(const cv::Mat& frame) = 0;

    private:
      const std::string name_;
      bool Start(const cv::Mat frame);

    signals:
      void signalFind(const IDetector* detector, const std::vector<cv::Rect> rectangles);
      void signalLose(const IDetector* detector);
  };

}

#endif // IOBSERVER_H
