cmake_minimum_required(VERSION 3.20)
project(MASTERCHIP8 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets CONFIG)
find_package(portable-file-dialogs CONFIG REQUIRED)

qt_standard_project_setup()


add_executable(MASTERCHIP8 src/main.cpp
        src/QtWindows/mainwindow.h src/QtWindows/mainwindow.cpp src/QtWindows/mainwindow.ui
        src/EmulatorSrc/utils.h)

include_directories(src/QtWindows src/EmulatorSrc)

target_link_libraries(MASTERCHIP8 PRIVATE
        Qt6::Widgets Qt6::OpenGLWidgets
        portable-file-dialogs::portable-file-dialogs)

set_target_properties(MASTERCHIP8 PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)
