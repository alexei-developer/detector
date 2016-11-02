#include <iostream>
#include <stdio.h>
#include <stdexcept>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>

#include "detect.h"
#include "core/core.h"


const std::string FIND_FILENAME_POSTFIX = "_find";

detect::DetectNotFound::DetectNotFound(const std::string& message) :
  message_(message)
{ }


const char* detect::DetectNotFound::what() const throw()
{
  return message_.c_str();
}



bool detect::init()
{

  return true;
}


void DrawText(cv::Mat& image)
{
    putText(image, "Hello OpenCV",
            cv::Point(20, 50),
            cv::FONT_HERSHEY_COMPLEX, 1, // font face and scale
            cv::Scalar(255, 255, 255), // white
            1, cv::LINE_AA); // line thickness and type
}


std::string DetectSaveImage(const cv::Mat& image, const std::string& path_image) {
  const std::string path_save = path_image + "_find";
  if (!imwrite(path_save, image))
    throw std::runtime_error("Fail save find elements image");

  return path_save;
}


std::string detect::DetectPedestrian(const std::string& path_image) {
  cv::Mat image = cv::imread(path_image);

  cv::Mat frame_gray;
  cv::cvtColor(image, frame_gray, cv::COLOR_BGR2GRAY);
  cv::equalizeHist(frame_gray, frame_gray);

  // параметры HOG-дескриптора и детектора
  int nbins = 9;
  cv::Size cellSize(8, 8);
  cv::Size blockSize(16, 16);
  cv::Size blockStride(8, 8);
  cv::Size winSize(64, 128);
  cv::Size winStride(4, 4);

  // создаем HOG-дескриптор/детектор
  cv::HOGDescriptor hog(winSize, blockSize,
                    blockStride, cellSize, nbins);

  // устанавливаем в качестве модели
  // линейную машину опорных векторов,
  // соответствующую размеру окна детектирования 64x128.
  hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

  // проверяем соответствие количества коэффициентов
  // гиперплоскости SVM и размера HOG-дескриптора
  // одного окна детектирования
  assert(hog.checkDetectorSize());

  // производим детектирование
  std::vector<cv::Rect> locations;
  std::vector<double> weights;
  hog.detectMultiScale(frame_gray, locations, weights, 0.0, winStride, cv::Size(),  1.05, 2., false);

  if (locations.size() == 0)
    throw DetectNotFound("Not found pedestrian");

  // отрисовываем срабатывания детектора на изображении
  for (size_t i = 0; i < locations.size(); ++i)
  {
    cv::Rect detection = locations[i];
    detection.x /= 2;
    detection.y /= 2;
    detection.width /= 2;
    detection.height /= 2;
    rectangle(image, detection, cv::Scalar(0, 0, 255), 2);
  }

  return DetectSaveImage(image, path_image);
}
