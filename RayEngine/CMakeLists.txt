
add_library(RayEngine STATIC
    src/RayEngine.h
    src/RayEngine/Core/Core.h
    src/RayEngine/Core/Core.cpp
 "src/RayEngine/Core/Application.h" "src/RayEngine/Core/Application.cpp")

target_include_directories(RayEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# to show folders in sln
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES
    src/RayEngine.h
    src/RayEngine/Core/Core.h
)
