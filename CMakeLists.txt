cmake_minimum_required(VERSION 3.16.0)

project(MASTERCHIP8 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(portable-file-dialogs CONFIG REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets Multimedia)
find_package(GTest REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

# --- Chip8 Core Library ---
add_library(Chip8Core STATIC
        src/EmulatorSrc/SuperChip8.cpp
        src/EmulatorSrc/SuperChip8.h
        src/EmulatorSrc/Display.cpp
        src/EmulatorSrc/Display.h
        src/EmulatorSrc/Memory.cpp
        src/EmulatorSrc/Memory.h
        src/EmulatorSrc/Registers.cpp
        src/EmulatorSrc/Registers.h
        src/EmulatorSrc/Keyboard.cpp
        src/EmulatorSrc/Keyboard.h
)

target_include_directories(Chip8Core PUBLIC src/EmulatorSrc)

set_target_properties(Chip8Core PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        POSITION_INDEPENDENT_CODE ON
)

# --- Qt Application ---
set(PROJECT_SOURCES
        src/main.cpp
        src/QtWindows/mainwindow.h src/QtWindows/mainwindow.cpp src/QtWindows/mainwindow.ui
        src/QtWindows/SettingsDialog.h src/QtWindows/SettingsDialog.cpp src/QtWindows/SettingsDialog.ui
        src/QtWindows/EmulatorDisplay.h src/QtWindows/EmulatorDisplay.cpp
)

if(WIN32)
    list(APPEND PROJECT_SOURCES resources/masterchip8.rc)
endif()

add_executable(MASTERCHIP8 ${PROJECT_SOURCES})

include_directories(src/QtWindows)

target_link_libraries(MASTERCHIP8 PRIVATE
        Chip8Core
        Qt6::Widgets Qt6::OpenGLWidgets Qt6::Multimedia
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
)

target_link_libraries(MASTERCHIP8_Tests PRIVATE Chip8Core GTest::gtest_main)

set_target_properties(MASTERCHIP8_Tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

include(GoogleTest)
gtest_discover_tests(MASTERCHIP8_Tests)

# --- Python Bindings ---
pybind11_add_module(masterchip8_py src/PythonBindings/bindings.cpp)
target_link_libraries(masterchip8_py PRIVATE Chip8Core)
set_target_properties(masterchip8_py PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    AUTOMOC OFF
)
