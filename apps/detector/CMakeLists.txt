set(PATH_RESOURCES ${CMAKE_INSTALL_PREFIX}/share/detector)
configure_file(config.h.in config.h @ONLY)

# Qt5
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package (Qt5Core REQUIRED)
find_package (Qt5Widgets REQUIRED)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")


add_executable(detector main.cpp  resources/resources.qrc)
target_link_libraries(detector detect Qt5::Widgets)

install(TARGETS detector DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources/images
        DESTINATION ${PATH_RESOURCES})
