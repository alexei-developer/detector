#include <stdexcept>

#include "pinled.h"


PinLED::PinLED(const int& pin) : pin_(pin)
{
  // "Failed BCM2835 init"
  if (!bcm2835_init())
    throw std::exception();
}


PinLED::~PinLED()
{
  bcm2835_close();
}


void PinLED::on() const
{
  bcm2835_gpio_write(pin_, HIGH);
}


void PinLED::off() const
{
  bcm2835_gpio_write(pin_, LOW);
}
