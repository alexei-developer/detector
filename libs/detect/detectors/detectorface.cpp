#include <opencv2/imgproc.hpp>

#include "core/core.h"
#include "detectorface.h"


const std::string PATH_CASCADE = OpenCV_PATH;
std::string FACE_CASCADE_NAME = PATH_CASCADE + "/lbpcascades/lbpcascade_frontalface.xml";
std::string EYES_CASCADE_NAME = PATH_CASCADE + "/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

cv::CascadeClassifier face_cascade;
cv::CascadeClassifier eyes_cascade;


detect::DetectorFace::DetectorFace() :
  IDetector("Face")
{
  if (!face_cascade.load(FACE_CASCADE_NAME))
    throw std::runtime_error("Error loading face cascade");

  if (!eyes_cascade.load(EYES_CASCADE_NAME))
    throw std::runtime_error("Error loading eyes cascade");
}


std::vector<cv::Rect> detect::DetectorFace::Detect(const cv::Mat& frame)
{
  std::vector<cv::Rect> faces;
  cv::Mat frame_gray;

  cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY );
  cv::equalizeHist(frame_gray, frame_gray);

  // Detect faces
  face_cascade.detectMultiScale(frame_gray, faces);

//  for( size_t i = 0; i < faces.size(); i++ )
//  {
//    cv::Mat faceROI = frame_gray(faces[i]);
//    std::vector<cv::Rect> eyes;

//    // In each face, detect eyes
//    eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
//    if( eyes.size() == 2)
//    {
//      // Draw the face
//      cv::Point center(faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2);
//      ellipse(image, center, cv::Size(faces[i].width/2, faces[i].height/2 ), 0, 0, 360, cv::Scalar( 255, 0, 0 ), 2, 8, 0);

//      for( size_t j = 0; j < eyes.size(); j++ )
//      { // Draw the eyes
//        cv::Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
//        int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
//        circle(image, eye_center, radius, cv::Scalar( 255, 0, 255 ), 3, 8, 0 );
//      }
//    }
//  }

  return faces;
}
