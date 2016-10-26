#include <exception>
#include <iostream>
#include <thread>

#include <QApplication>
#include <QtGlobal>

#include "lib/detector.h"
#include "lib/pinled.h"
#include "config.h"


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  qDebug() << a.arguments();

  const bool pinledon = a.arguments().contains("pinledon");
  const bool pinledoff = a.arguments().contains("pinledoff");
  const bool sleepon = a.arguments().contains("sleepon");

  try {

    if (pinledon)
    {
      qInfo() << "Pin led on";
      PinLED pin;
      pin.on();
      if (sleepon)
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    else if (pinledoff)
    {
      qInfo() << "Pin led off";
      PinLED pin;
      pin.off();
      if (sleepon)
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    else
    {
      if (!init())
        return 1;
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
