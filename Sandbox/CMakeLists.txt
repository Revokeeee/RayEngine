add_executable(Sandbox
    src/main.cpp
)

target_link_libraries(Sandbox PRIVATE RayEngine)
target_include_directories(Sandbox PRIVATE ../RayEngine/include)
