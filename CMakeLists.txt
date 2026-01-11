cmake_minimum_required(VERSION 3.20)

project(editor)

find_package(stb REQUIRED)

add_executable(editor src/main.cpp)

target_link_libraries(editor PRIVATE stb::stb)
