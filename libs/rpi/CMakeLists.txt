if (DETECTOR_WITH_RPI)
    add_definitions(-DWITH_RPI=1)

    include_directories("${BCM2835_INCLUDE}")

    add_library(rpi STATIC pinled.h pinled.cpp)
    target_link_libraries(rpi bcm2835)
endif()
