add_library(core STATIC
    image/ImageData.cpp
    image/Image.cpp
    transformation/grayscale.cpp
    transformation/inversion.cpp
    transformation/binarization.cpp
    filter/sobel.cpp
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
