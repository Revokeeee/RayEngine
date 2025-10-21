
add_library(RayEngine STATIC
    src/RayEngine.h
    src/RayEngine/Core/Core.h
    src/RayEngine/Core/Application.h
    src/RayEngine/Core/Application.cpp
    src/RayEngine/Core/Log.h
    src/RayEngine/Core/Log.cpp

 "src/RayEngine/Core/Log.h"
 "src/RayEngine/Core/Log.cpp" 
 "src/RayEngine/Core/Time.h" 
 "src/RayEngine/Core/Profiler.h" "src/RayEngine/Core/Layer.h" "src/RayEngine/Core/LayerStack.h" "src/RayEngine/Core/LayerStack.cpp")

target_include_directories(RayEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(RayEngine PUBLIC spdlog::spdlog)

target_compile_definitions(RayEngine PUBLIC
    $<$<CONFIG:Debug>:RAY_DEBUG>     
    $<$<CONFIG:Release>:RAY_RELEASE>   
)

