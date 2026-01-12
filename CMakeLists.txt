cmake_minimum_required(VERSION 3.20)

project(editor)

find_package(stb REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets UiTools)

add_executable(editor src/main.cpp src/Image.cpp src/ToQImage.cpp)

target_link_libraries(editor PRIVATE stb::stb Qt5::Widgets Qt5::UiTools)
