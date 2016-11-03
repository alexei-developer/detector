#include <exception>
#include <iostream>
#include <thread>

#include <QCoreApplication>
#include <QStringList>
#include <QCommandLineParser>

#include "config.h"
#include "core/core.h"
#include "detect/video.h"
#include "detect/detectors/detectorface.h"


int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);
  QCoreApplication::setApplicationName("Detector");
  QCoreApplication::setApplicationVersion(DETECTOR_VERSION);

  LOG_INFO << "Detector version:" << DETECTOR_VERSION;
  LOG_INFO << "Qt version:" << qVersion();

  LOG_DEBUG << "Arguments:" << app.arguments().join(" ").toStdString();

  QCommandLineParser parser;
  parser.addVersionOption();
  parser.addHelpOption();

  parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source for video capture."));
  parser.process(app);

  const QStringList sources = parser.positionalArguments();
  if (sources.size() != 1) {
    LOG_ERROR << "Need one source for video capture";
    parser.showHelp(1);
  }

  detect::DetectorFace detectorFace;
  try {
    QString url = sources.first();
    bool is_device = false;
    int device = url.toInt(&is_device);

    std::shared_ptr<detect::Video> video;
    if (is_device)
      video = std::make_shared<detect::Video>(device);
    else
      video = std::make_shared<detect::Video>(url.toStdString());

    video->AddDetector(&detectorFace);
    video->Start();

    LOG_INFO << "Press <enter> key for finish...";
    std::cin.get();
  }
  catch (const std::exception& e) {
    LOG_CRITICAL << "Error: " << e.what();
    return 1;
  }
  catch (...) {
    LOG_CRITICAL << "Undefined error";
    return 2;
  }

  return 0;
}
