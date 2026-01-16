cmake_minimum_required(VERSION 3.20)

project(editor)

find_package(stb REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets UiTools)

add_subdirectory(core)

add_executable(editor 
src/main.cpp 
src/MainWindow/Presenter.cpp
src/MainWindow/Model.cpp
src/MainWindow/View.cpp)

target_link_libraries(editor PRIVATE core stb::stb Qt5::Widgets Qt5::UiTools)
