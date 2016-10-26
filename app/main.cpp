#include <exception>
#include <iostream>
#include <thread>

#include <QApplication>
#include <QtGlobal>
#include <QDebug>

#include "config.h"
#include "lib/detector.h"

#ifdef WITH_RPI
#include "lib/pinled.h"
#endif

#if (QT_VERSION < QT_VERSION_CHECK(5,5,0))
#include <sstream>
class qInfo
{
    std::stringstream stream;

    public:
        ~qInfo()
        {
          std::cout << stream.str() << std::endl;
        }

        template <typename T>
        qInfo &operator<<(T t) { stream << t << " "; return *this; }

        inline qInfo &operator<<(bool t) { stream << (t ? "True" : "False"); return *this; }
};
#endif


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  qInfo() << "Detector version:" << VERSION_TXT;
  qInfo() << "Qt version:" << qVersion();

  qDebug() << "Arguments:" << a.arguments();

  const bool pinledon = a.arguments().contains("pinledon");
  const bool pinledoff = a.arguments().contains("pinledoff");
  const bool sleepon = a.arguments().contains("sleepon");

  try {

#ifdef WITH_RPI
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
#endif

    if (!init())
      return 1;
    photo_process(PATH_RESOURCES "/images/faces.jpg");

#ifdef WITH_RPI
    }
#endif

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
