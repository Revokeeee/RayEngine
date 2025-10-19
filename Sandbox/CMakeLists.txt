add_executable(Sandbox
    src/main.cpp
)

target_link_libraries(Sandbox PRIVATE RayEngine)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES
    src/main.cpp
)
