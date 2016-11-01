set(PATH_RESOURCES ${CMAKE_INSTALL_PREFIX}/share/detector)
configure_file(config.h.in config.h @ONLY)

# Qt5
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)

add_executable(detector main.cpp  resources/resources.qrc)
target_link_libraries(detector detect Qt5::Core core)

install(TARGETS detector DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources/images
        DESTINATION ${PATH_RESOURCES})
