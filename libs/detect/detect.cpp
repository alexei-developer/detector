#include <iostream>
#include <stdio.h>
#include <stdexcept>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>

#include "detect.h"
#include "core/core.h"


const std::string FIND_FILENAME_POSTFIX = "_find";

const std::string PATH_CASCADE = OpenCV_PATH;
std::string FACE_CASCADE_NAME = PATH_CASCADE + "/lbpcascades/lbpcascade_frontalface.xml";
std::string EYES_CASCADE_NAME = PATH_CASCADE + "/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
cv::CascadeClassifier face_cascade;
cv::CascadeClassifier eyes_cascade;


void video(cv::VideoCapture& capture);

detect::DetectNotFound::DetectNotFound(const std::string& message) :
  message_(message)
{ }


const char* detect::DetectNotFound::what() const throw()
{
  return message_.c_str();
}



bool detect::init()
{
  if (!face_cascade.load(FACE_CASCADE_NAME)) {
    LOG_CRITICAL << "Error loading face cascade\n";
    return false;
  }

  if (!eyes_cascade.load(EYES_CASCADE_NAME)) {
    LOG_CRITICAL << "Error loading eyes cascade\n";
    return false;
  }

  return true;
}


void detect::video_process(const std::string& url)
{
  cv::VideoCapture capture;
  capture.open(url);

  video(capture);
}


void detect::video_process(const int& device)
{
  cv::VideoCapture capture;
  capture.open(device);

  video(capture);
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



std::string detect::DetectFace(const std::string& path_image)
{
  cv::Mat image = cv::imread(path_image);

  std::vector<cv::Rect> faces;
  cv::Mat frame_gray;

  cv::cvtColor(image, frame_gray, cv::COLOR_BGR2GRAY );
  cv::equalizeHist( frame_gray, frame_gray );

  // Detect faces
  face_cascade.detectMultiScale(frame_gray, faces );

  if (faces.size() == 0)
    throw DetectNotFound("Not found faces");

  for( size_t i = 0; i < faces.size(); i++ )
  {
    cv::Mat faceROI = frame_gray(faces[i]);
    std::vector<cv::Rect> eyes;

    // In each face, detect eyes
    eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
    if( eyes.size() == 2)
    {
      // Draw the face
      cv::Point center(faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2);
      ellipse(image, center, cv::Size(faces[i].width/2, faces[i].height/2 ), 0, 0, 360, cv::Scalar( 255, 0, 0 ), 2, 8, 0);

      for( size_t j = 0; j < eyes.size(); j++ )
      { // Draw the eyes
        cv::Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
        int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
        circle(image, eye_center, radius, cv::Scalar( 255, 0, 255 ), 3, 8, 0 );
      }
    }
  }

  return DetectSaveImage(image, path_image);
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


void video(cv::VideoCapture& capture)
{
  cv::Mat image;
  if(capture.isOpened())
  {
    std::cout << "Capture is opened" << std::endl;
    for(;;)
    {
      capture >> image;
      if(image.empty())
        break;

      // DetectFace(image);
      //detectPedestrian(image);
      cv::imshow("Sample", image);
      if(cv::waitKey(10) >= 0)
        break;
    }
  }
  else
  {
    std::cout << "No capture" << std::endl;
    image = cv::Mat::zeros(480, 640, CV_8UC1);
    cv::imshow("Sample", image);
    cv::waitKey(0);
  }
}


