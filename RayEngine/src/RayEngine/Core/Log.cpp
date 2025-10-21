#include "Log.h"

namespace RayEngine {

    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    void Log::Init(const char* pattern) {
        try {
			// Set log pattern
            spdlog::set_pattern(pattern);

            // Core logger (engine)
            s_CoreLogger = spdlog::stdout_color_mt("RAYENGINE");

            // Client logger (sandbox / user)
            s_ClientLogger = spdlog::stdout_color_mt("APP");

#ifdef RAY_DEBUG
            // Debug
            s_CoreLogger->set_level(spdlog::level::trace);
            s_ClientLogger->set_level(spdlog::level::trace);
#else RAY_RELEASE
            // TODO: Release
            s_CoreLogger->set_level(spdlog::level::warn);
            s_ClientLogger->set_level(spdlog::level::warn);
#endif

            // Optional
            spdlog::flush_on(spdlog::level::err);
        }
        catch (const spdlog::spdlog_ex& ex) {
			// Fallcack if log initialization fails
            printf("Log init failed: %s\n", ex.what());
        }
    }


    std::shared_ptr<spdlog::logger>& Log::GetCoreLogger() { return s_CoreLogger; }
    std::shared_ptr<spdlog::logger>& Log::GetClientLogger() { return s_ClientLogger; }

} 
