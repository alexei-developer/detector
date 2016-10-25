#ifndef PINLED_H
#define PINLED_H

#include <bcm2835.h>


class PinLED
{
  public:
    PinLED(const int& pin = RPI_V2_GPIO_P1_03);
    ~PinLED();

    void on() const;
    void off() const;

  private:
    const int pin_;
};

#endif // PIN_H
