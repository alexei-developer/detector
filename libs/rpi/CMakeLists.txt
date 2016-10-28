add_library(rpi STATIC pinled.h pinled.cpp)
target_link_libraries(rpi bcm2835)
