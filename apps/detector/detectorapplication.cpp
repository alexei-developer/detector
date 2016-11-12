#include <signal.h>

#include "detectorapplication.h"
#include "core/core.h"


DetectorApplication::DetectorApplication(int& argc, char** argv) :
  QCoreApplication(argc, argv)
{
  QCoreApplication::setApplicationName("Detector");
  QCoreApplication::setApplicationVersion(DETECTOR_VERSION);

  LOG_INFO << "Detector version:" << DETECTOR_VERSION;
  LOG_INFO << "Qt version:" << qVersion();

  LOG_DEBUG << "Arguments:" << arguments().join(" ").toStdString();

  QCommandLineParser parser;
  parser.addVersionOption();
  parser.addHelpOption();

  parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source for video capture."));

  QCommandLineOption detectorMotion("m", QCoreApplication::translate("main", "Detect any motion"));
  parser.addOption(detectorMotion);

  QCommandLineOption detectorFace("f", QCoreApplication::translate("main", "Detect faces"));
  parser.addOption(detectorFace);

  QCommandLineOption pathSave("p", QCoreApplication::translate("main", "Path for save video files"), "path");
  parser.addOption(pathSave);

  parser.process(*this);


  const QStringList sources = parser.positionalArguments();
  if (sources.size() != 1) {
    LOG_ERROR << "Need one source for video capture";
    parser.showHelp(1);
  }
  sourceCapture_ = sources.first();

  detectorMotion_ = parser.isSet(detectorMotion);
  detectorFace_   = parser.isSet(detectorFace);

  pathSave_ = parser.value(pathSave);

  signal(SIGINT,  WaitExitKey);
  signal(SIGTERM, WaitExitKey);
}


DetectorApplication::~DetectorApplication()
{
  if (threadWaitExitKey_.joinable())
    threadWaitExitKey_.join();
}


QString DetectorApplication::SourceCapture() const
{
  return sourceCapture_;
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