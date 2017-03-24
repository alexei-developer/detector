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
  detector::DetectorApplication app(argc, argv);

  detector::Detector detector;
  detector.Start();
  LOG_INFO << "Successfull start";

  return app.exec();
}
