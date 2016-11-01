find_package(Qt5Core REQUIRED)

add_executable(pinled main.cpp)
target_link_libraries(pinled rpi core Qt5::Core)

install(TARGETS pinled DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
