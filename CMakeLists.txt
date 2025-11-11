cmake_minimum_required(VERSION 3.20)
project(MASTERCHIP8 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets CONFIG)

qt_standard_project_setup()


add_executable(MASTERCHIP8 src/main.cpp)

target_link_libraries(MASTERCHIP8 PRIVATE Qt6::Widgets)

get_target_property(QT_LIB_DIR Qt6::Core IMPORTED_LOCATION_RELEASE)
get_filename_component(QT_DIR "${QT_LIB_DIR}" DIRECTORY)
set(QT_PLUGIN_PATH "${QT_DIR}/../plugins")

add_custom_command(TARGET MASTERCHIP8 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${QT_PLUGIN_PATH}/platforms"
        $<TARGET_FILE_DIR:MASTERCHIP8>/platforms
)

set_target_properties(MASTERCHIP8 PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)
