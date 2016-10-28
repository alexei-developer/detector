message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OPENCV_VERSION}")

add_definitions(-DOpenCV_PATH="${CMAKE_INSTALL_PREFIX}/share/OpenCV")

include_directories("${opencv_core_SOURCE_DIR}/include")
include_directories("${opencv_highgui_SOURCE_DIR}/include")
include_directories("${opencv_imgproc_SOURCE_DIR}/include")
include_directories("${opencv_imgcodecs_SOURCE_DIR}/include")
include_directories("${opencv_videoio_SOURCE_DIR}/include")
include_directories("${opencv_objdetect_SOURCE_DIR}/include")

add_library(detect STATIC detect.cpp detect.h)
target_link_libraries(detect opencv_core opencv_highgui opencv_imgproc
                             opencv_videoio opencv_objdetect opencv_imgcodecs)

