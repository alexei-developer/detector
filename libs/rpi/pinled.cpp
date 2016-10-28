#include <stdexcept>

#include "pinled.h"


PinLED::PinLED(const int& pin) : pin_(pin)
{
  if (!bcm2835_init())
    throw std::exception();

  bcm2835_gpio_fsel(pin_, BCM2835_GPIO_FSEL_OUTP);
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
