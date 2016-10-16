#include <QApplication>

#include <lib/detector.h>

#include "config.h"


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  if (!init())
    return 1;

  photo_process(PATH_RESOURCES "/images/faces.jpg");

  return 0;
}
