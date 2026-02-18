add_library(core STATIC
    Image.cpp
    transformations/grayscale.cpp
    transformations/inversion.cpp
    transformations/binarization.cpp
)

find_package(OpenMP REQUIRED)

target_include_directories(core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(core
    PUBLIC
        Qt5::Widgets
        OpenMP::OpenMP_CXX
)
