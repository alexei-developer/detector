#ifndef DETECTORAPPLICATION_H
#define DETECTORAPPLICATION_H

#include <thread>
#include <QtCore>

#include "constant.h"


namespace detector {

  class DetectorApplication : public QCoreApplication
  {
      Q_OBJECT

    public:
      DetectorApplication(int &argc, char **argv);
      ~DetectorApplication();

      SourceCaptureArray SourceCapture() const;
      QString pathSave() const;
      bool DetectorMotion() const;
      bool DetectorFace() const;

    private:
      SourceCaptureArray source_captures;
      QString pathSave_;
      bool detectorMotion_;
      bool detectorFace_;

      std::thread threadWaitExitKey_;

      static void WaitExitKey(int sig);
  };

}

#endif // DETECTORAPPLICATION_H
