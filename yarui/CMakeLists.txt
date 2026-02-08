find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_library(yarui INTERFACE)

target_include_directories(yarui
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(yarui
    INTERFACE
        Qt5::Widgets
)
