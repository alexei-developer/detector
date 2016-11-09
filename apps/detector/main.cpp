#include <exception>
#include <iostream>
#include <thread>

#include <QCoreApplication>
#include <QStringList>
#include <QCommandLineParser>

#include "core/core.h"
#include "detectorapplication.h"
#include "detector.h"


int main(int argc, char *argv[])
{
  DetectorApplication app(argc, argv);

  Detector detector(app.SourceCapture(), app.pathSave(), app.DetectorMotion(), app.DetectorFace());
  detector.Start();

  LOG_INFO << "Successfull start";

  return app.exec();
}
