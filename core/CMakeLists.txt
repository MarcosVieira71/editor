add_library(core STATIC
    Image.cpp
    filters/Grayscale.cpp
    filters/Inversion.cpp
)

target_include_directories(core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(core
    PUBLIC
        stb::stb
        Qt5::Widgets
)
