add_library(infra STATIC
    threading/ThreadPool.cpp
    images/ImageLoader.cpp
)

find_package(stb REQUIRED)

target_include_directories(infra
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(infra
    PUBLIC
        stb::stb
)
