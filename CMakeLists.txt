cmake_minimum_required(VERSION 3.16.0)

project(MASTERCHIP8 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(portable-file-dialogs CONFIG REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(GTest REQUIRED)

add_executable(MASTERCHIP8 src/main.cpp
        src/QtWindows/mainwindow.h src/QtWindows/mainwindow.cpp src/QtWindows/mainwindow.ui
        src/QtWindows/SettingsDialog.h src/QtWindows/SettingsDialog.cpp src/QtWindows/SettingsDialog.ui
        src/QtWindows/EmulatorDisplay.h src/QtWindows/EmulatorDisplay.cpp
        src/EmulatorSrc/SuperChip8.cpp
        src/EmulatorSrc/SuperChip8.h
        src/EmulatorSrc/Display.cpp
        src/EmulatorSrc/Display.h
        src/EmulatorSrc/Memory.cpp
        src/EmulatorSrc/Memory.h
        src/EmulatorSrc/Registers.cpp
        src/EmulatorSrc/Registers.h
)

include_directories(src/QtWindows src/EmulatorSrc)

target_link_libraries(MASTERCHIP8 PRIVATE
        Qt6::Widgets Qt6::OpenGLWidgets
        portable-file-dialogs::portable-file-dialogs)

set_target_properties(MASTERCHIP8 PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

# --- Unit Tests ---
enable_testing()

add_executable(MASTERCHIP8_Tests
        tests/test_main.cpp
        tests/test_memory.cpp
        tests/test_display.cpp
        tests/test_superchip8.cpp
        tests/test_registers.cpp
        src/EmulatorSrc/SuperChip8.cpp
        src/EmulatorSrc/SuperChip8.h
        src/EmulatorSrc/Display.cpp
        src/EmulatorSrc/Display.h
        src/EmulatorSrc/Memory.cpp
        src/EmulatorSrc/Memory.h
        src/EmulatorSrc/Registers.cpp
        src/EmulatorSrc/Registers.h
)

target_include_directories(MASTERCHIP8_Tests PRIVATE src/EmulatorSrc)

target_link_libraries(MASTERCHIP8_Tests PRIVATE GTest::gtest_main)

set_target_properties(MASTERCHIP8_Tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

include(GoogleTest)
gtest_discover_tests(MASTERCHIP8_Tests)
