#include <signal.h>

#include "detectorapplication.h"
#include "core/core.h"


using namespace detector;

DetectorApplication::DetectorApplication(int& argc, char** argv) :
  QCoreApplication(argc, argv)
{
  QCoreApplication::setApplicationName("Detector");
  QCoreApplication::setApplicationVersion(DETECTOR_VERSION);

  LOG_INFO << "Detector version:" << DETECTOR_VERSION;
  LOG_INFO << "Qt version:" << qVersion();

  signal(SIGINT,  WaitExitKey);
  signal(SIGTERM, WaitExitKey);
}


DetectorApplication::~DetectorApplication()
{
  if (threadWaitExitKey_.joinable())
    threadWaitExitKey_.join();
}


void DetectorApplication::WaitExitKey(int sig)
{
  LOG_INFO << "Shutdown detector";
  QCoreApplication::exit();
}
