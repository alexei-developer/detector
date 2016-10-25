project(lib)

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OPENCV_VERSION}")


set(CURRENT_SRC detector.cpp detector.h)
set(CURRENT_LIBS opencv_core opencv_highgui opencv_imgproc
                 opencv_videoio opencv_objdetect opencv_imgcodecs)


# rpi
if (DETECTOR_WITH_RPI)
    add_definitions(-DWITH_RPI=1)

    include_directories("${BCM2835_INCLUDE}")

    list(APPEND CURRENT_SRC pinled.h pinled.cpp)
    list(APPEND CURRENT_LIBS ${BCM2835_LIBS})
endif()


# main
add_definitions(-DOpenCV_PATH="${OpenCV_SOURCE_DIR}")

include_directories("${opencv_core_SOURCE_DIR}/include")
include_directories("${opencv_highgui_SOURCE_DIR}/include")
include_directories("${opencv_imgproc_SOURCE_DIR}/include")
include_directories("${opencv_imgcodecs_SOURCE_DIR}/include")
include_directories("${opencv_videoio_SOURCE_DIR}/include")
include_directories("${opencv_objdetect_SOURCE_DIR}/include")

add_library(detect STATIC ${CURRENT_SRC})
target_link_libraries(detect ${CURRENT_LIBS})

if (DETECTOR_WITH_RPI)
    add_dependencies(detect bcm2835)
endif()
