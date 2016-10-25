#include <exception>
#include <iostream>

#include <QApplication>
#include <QDebug>

#include "lib/detector.h"
#include "lib/pinled.h"
#include "config.h"


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  qDebug() << a.arguments();

  if (!init())
    return 1;

  try {
    if (a.arguments().indexOf("pinledon"))
    {
      qInfo() << "Pin led on";
      PinLED pin;
      pin.on();
    }
    if (a.arguments().indexOf("pinledoff"))
    {
      qInfo() << "Pin led off";
      PinLED pin;
      pin.off();
    }
    else
    {
      photo_process(PATH_RESOURCES "/images/faces.jpg");
    }
  }
  catch (std::exception& e) {
    qCritical() << "Error: " << e.what();
    return 1;
  }
  catch (...) {
    return 2;
  }


  return 0;
}
