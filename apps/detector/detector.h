#ifndef DETECTOR_H
#define DETECTOR_H

#include <QtCore>

#include "detect/video.h"


class Detector
{
  public:
    Detector(const QString& url,const QString& pathSave = "",
             const bool& detectMotion = false, const bool& detectFace = false);
    bool Start();

  private:
    std::list< std::shared_ptr<detect::IDetector> > detectors_;
    std::shared_ptr<detect::VideoCapture> video_;
    std::shared_ptr<detect::VideoWriter> writer_;
};

#endif // DETECTOR_H
