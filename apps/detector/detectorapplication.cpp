#include <signal.h>

#include "detectorapplication.h"
#include "constant.h"
#include "core/core.h"

using namespace detector;

DetectorApplication::DetectorApplication(int& argc, char** argv) :
  QCoreApplication(argc, argv)
{
  QCoreApplication::setApplicationName("Detector");
  QCoreApplication::setApplicationVersion(DETECTOR_VERSION);

  LOG_INFO << "Detector version:" << DETECTOR_VERSION;
  LOG_INFO << "Qt version:" << qVersion();

  LOG_DEBUG << "Arguments:" << arguments().join(" ").toStdString();

  QSettings settings("setting.ini", QSettings::IniFormat);

  // General
  pathSave_ = settings.value("path_save", "").toString();

  // Cams
  settings.beginGroup("Cams");
  for(int i = 0; i < kMaxCam; ++i) {
    source_captures[i] = settings.value("source_" + QString::number(i), "").toString();
  }
  settings.endGroup();

  // Detectors
  settings.beginGroup("Detectors");
  detectorMotion_ = settings.value("motion", false).toBool();
  detectorFace_   = settings.value("face", false).toBool();
  settings.endGroup();

  signal(SIGINT,  WaitExitKey);
  signal(SIGTERM, WaitExitKey);
}


DetectorApplication::~DetectorApplication()
{
  if (threadWaitExitKey_.joinable())
    threadWaitExitKey_.join();
}


SourceCaptureArray DetectorApplication::SourceCapture() const
{
  return source_captures;
}


QString DetectorApplication::pathSave() const
{
  return pathSave_;
}


bool DetectorApplication::DetectorMotion() const
{
  return detectorMotion_;
}


bool DetectorApplication::DetectorFace() const
{
  return detectorFace_;
}


void DetectorApplication::WaitExitKey(int sig)
{
  LOG_INFO << "Shutdown detector";
  QCoreApplication::exit();
}
