add_library(RayEngine STATIC
    src/Core.cpp

    include/RayEngine/Core.h
)

target_include_directories(RayEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

