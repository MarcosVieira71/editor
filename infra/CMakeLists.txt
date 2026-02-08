add_library(infra STATIC
    ThreadPool.cpp
)

target_include_directories(infra
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)
