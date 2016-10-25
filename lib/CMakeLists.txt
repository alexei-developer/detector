project(lib)

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OPENCV_VERSION}")


set(SOURCE_LIB detector.cpp detector.h)


# rpi
if (DETECTOR_WITH_RPI)
    add_definitions(-DWITH_RPI=1)
    list(APPEND SOURCE_LIB pin.h pin.cpp)
endif()


# main
add_definitions(-DOpenCV_PATH="${OpenCV_SOURCE_DIR}")

include_directories("${opencv_core_SOURCE_DIR}/include")
include_directories("${opencv_highgui_SOURCE_DIR}/include")
include_directories("${opencv_imgproc_SOURCE_DIR}/include")
include_directories("${opencv_imgcodecs_SOURCE_DIR}/include")
include_directories("${opencv_videoio_SOURCE_DIR}/include")
include_directories("${opencv_objdetect_SOURCE_DIR}/include")


add_library(detect STATIC ${SOURCE_LIB})

target_link_libraries(detect opencv_core opencv_highgui opencv_imgproc
                             opencv_videoio opencv_objdetect opencv_imgcodecs)
