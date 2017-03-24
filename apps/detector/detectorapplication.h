#ifndef DETECTORAPPLICATION_H
#define DETECTORAPPLICATION_H

#include <thread>
#include <QtCore>


namespace detector {

  class DetectorApplication : public QCoreApplication
  {
      Q_OBJECT

    public:
      DetectorApplication(int &argc, char **argv);
      ~DetectorApplication();

    private:
      std::thread threadWaitExitKey_;

      static void WaitExitKey(int sig);
  };

}

#endif // DETECTORAPPLICATION_H
