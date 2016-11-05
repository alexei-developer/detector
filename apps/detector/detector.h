#ifndef DETECTOR_H
#define DETECTOR_H

#include <QtCore>

#include "detect/video.h"
#include "detect/detectors/detectorface.h"
#include "detect/detectors/detectormotion.h"

class Detector
{
  public:
    Detector(const QString& url);
    bool Start();

  private:
    detect::DetectorFace detectorFace;
    detect::DetectorMotion detectorMotion;

    std::shared_ptr<detect::VideoCapture> video;
};

#endif // DETECTOR_H
