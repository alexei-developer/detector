# OpenCV
add_definitions(-DOpenCV_PATH="${CMAKE_INSTALL_PREFIX}/share/OpenCV")

include_directories("${opencv_core_SOURCE_DIR}/include")
include_directories("${opencv_highgui_SOURCE_DIR}/include")
include_directories("${opencv_imgproc_SOURCE_DIR}/include")
include_directories("${opencv_imgcodecs_SOURCE_DIR}/include")
include_directories("${opencv_videoio_SOURCE_DIR}/include")
include_directories("${opencv_objdetect_SOURCE_DIR}/include")


# detect
add_library(detect STATIC
  detect.h detect.cpp
  video.h video.cpp
  iobserver.h iobserver.cpp
  detectors/detectorface.h
  detectors/detectorface.cpp)

target_link_libraries(detect
  core pthread
  opencv_core opencv_highgui opencv_imgproc
  opencv_videoio opencv_objdetect opencv_imgcodecs)

