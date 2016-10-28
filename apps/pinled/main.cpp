#include <exception>
#include <iostream>
#include <thread>

#include <QCoreApplication>
#include <QStringList>

#include "rpi/pinled.h"
#include "core/core.h"


int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  LOG_INFO << "PinLED version:" << VERSION_TXT;

  try {
    LOG_DEBUG << "Arguments:" << a.arguments();

    const bool pinledon  = a.arguments().contains("on");
    const bool pinledoff = a.arguments().contains("off");
    const bool sleepon   = a.arguments().contains("sleepon");

    if ( ! (pinledon || pinledoff))
      return false;

    PinLED pin;

    if (pinledon)
    {
      LOG_INFO << "Pin led on";
      pin.on();
    }
    else if (pinledoff)
    {
      LOG_INFO << "Pin led off";
      pin.off();
    }

    if (sleepon)
      std::this_thread::sleep_for(std::chrono::seconds(3));

    return 0;
  }
  catch (std::exception& e) {
    LOG_CRITICAL << "Error: " << e.what();
    return 1;
  }
  catch (...) {
    return 2;
  }

  return 0;
}
