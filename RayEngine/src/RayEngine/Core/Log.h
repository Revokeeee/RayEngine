#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace RayEngine {

    class Log {
    public:
        static void Init(const char* pattern = "[%T] [%^%l%$] %v");
        static void ShutDown() { spdlog::shutdown(); }
        static std::shared_ptr<spdlog::logger>& GetCoreLogger();
        static std::shared_ptr<spdlog::logger>& GetClientLogger();

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

    // macros
#define RAY_CORE_TRACE(...)    ::RayEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RAY_CORE_INFO(...)     ::RayEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RAY_CORE_WARN(...)     ::RayEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RAY_CORE_ERROR(...)    ::RayEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RAY_CORE_CRITICAL(...) ::RayEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define RAY_CLIENT_TRACE(...)    ::RayEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RAY_CLIENT_INFO(...)     ::RayEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define RAY_CLIENT_WARN(...)     ::RayEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RAY_CLIENT_ERROR(...)    ::RayEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define RAY_CLIENT_CRITICAL(...) ::RayEngine::Log::GetClientLogger()->critical(__VA_ARGS__)

}