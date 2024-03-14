cmake_minimum_required(VERSION 3.16)

project(untitled6 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick UiPlugin REQUIRED)

qt_add_executable(appuntitled6
    main.cpp piorganInterface.cpp piorgan.cpp
)

qt_add_qml_module(appuntitled6
    URI untitled6
    VERSION 1.0
    QML_FILES main.qml 
)

set_target_properties(appuntitled6 PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

find_package(ALSA REQUIRED)
find_library(FLUID_LIBRARIES NAMES fluidsynth)

target_link_libraries(appuntitled6
    PRIVATE Qt6::Quick Qt6::UiPlugin ${FLUID_LIBRARIES} ${ALSA_LIBRARY})

install(TARGETS appuntitled6
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
