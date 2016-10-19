#include "detector.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

const string PATH_CASCADE = OpenCV_INSTALL_PATH "/share/OpenCV";
string face_cascade_name = PATH_CASCADE + "/lbpcascades/lbpcascade_frontalface.xml";
string eyes_cascade_name = PATH_CASCADE + "/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

bool init();

void video_process();
void photo_process();

void detectFace(Mat& frame);
void detectPedestrian(Mat& frame) ;


bool init() {
  if( !face_cascade.load( face_cascade_name ) ) {
    cerr << "Error loading face cascade\n";
    return false;
  }

  if( !eyes_cascade.load( eyes_cascade_name ) ){
    cerr << "Error loading eyes cascade\n";
    return false;
  }

  return true;
}


void video_process()
{
  Mat image;
  VideoCapture capture;
  capture.open(0);
  // capture.open("rtsp://192.168.43.243:554/user=admin&password=$channel=1&stream=0.sdp?");
  if(capture.isOpened())
  {
    cout << "Capture is opened" << endl;
    for(;;)
    {
      capture >> image;
      if(image.empty())
        break;

      detectFace(image);
      //detectPedestrian(image);
      imshow("Sample", image);
      if(waitKey(10) >= 0)
        break;
    }
  }
  else
  {
    cout << "No capture" << endl;
    image = Mat::zeros(480, 640, CV_8UC1);
    imshow("Sample", image);
    waitKey(0);
  }
}


void photo_process(const string& path) {

  Mat image = imread(path);
  detectFace(image);
//  namedWindow("Sample", WINDOW_NORMAL);
//  imshow("Sample", image);
//  waitKey(0);
}


void drawText(Mat & image)
{
    putText(image, "Hello OpenCV",
            Point(20, 50),
            FONT_HERSHEY_COMPLEX, 1, // font face and scale
            Scalar(255, 255, 255), // white
            1, LINE_AA); // line thickness and type
}


void detectFace(Mat& frame)
{
    std::vector<Rect> faces;
    Mat frame_gray;

    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    face_cascade.detectMultiScale(frame_gray, faces );

    cout << faces.size() << endl;

    for( size_t i = 0; i < faces.size(); i++ )
    {
        Mat faceROI = frame_gray( faces[i] );
        std::vector<Rect> eyes;

        //-- In each face, detect eyes
        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(30, 30) );
        if( eyes.size() == 2)
        {
            //-- Draw the face
            Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
            ellipse( frame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 0 ), 2, 8, 0 );

            for( size_t j = 0; j < eyes.size(); j++ )
            { //-- Draw the eyes
                Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
                int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
                circle( frame, eye_center, radius, Scalar( 255, 0, 255 ), 3, 8, 0 );
            }
        }
    }
}


void detectPedestrian(Mat& frame) {
  Mat frame_gray;
  cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  // параметры HOG-дескриптора и детектора
  int nbins = 9;
  Size cellSize(8, 8);
  Size blockSize(16, 16);
  Size blockStride(8, 8);
  Size winSize(64, 128);
  Size winStride(4, 4);

  // создаем HOG-дескриптор/детектор
  HOGDescriptor hog(winSize, blockSize,
  blockStride, cellSize, nbins);

  // устанавливаем в качестве модели
  // линейную машину опорных векторов,
  // соответствующую размеру окна детектирования 64x128.
  hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

  // проверяем соответствие количества коэффициентов
  // гиперплоскости SVM и размера HOG-дескриптора
  // одного окна детектирования
  assert(hog.checkDetectorSize());

  // производим детектирование
  vector<Rect> locations;
  vector<double> weights;
  hog.detectMultiScale(frame, locations, weights, 0.0, winStride, Size(),  1.05, 2., false);

  // отрисовываем срабатывания детектора на изображении
  for (size_t i = 0; i < locations.size(); ++i)
  {
    Rect detection = locations[i];
    detection.x /= 2;
    detection.y /= 2;
    detection.width /= 2;
    detection.height /= 2;
    rectangle(frame, detection, Scalar(0, 0, 255), 2);
  }
}
