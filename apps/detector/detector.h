#ifndef DETECTOR_H
#define DETECTOR_H

#include <QtCore>

#include "detect/video.h"


class Detector
{
  public:
    Detector(const QString& url, const bool& detectMotion, const bool& detectFace);
    bool Start();

  private:
    std::list< std::shared_ptr<detect::IDetector> > detectors_;
    std::shared_ptr<detect::VideoCapture> video_;
};

#endif // DETECTOR_H
