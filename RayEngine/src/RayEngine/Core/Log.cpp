#include "Log.h"

namespace RayEngine {

    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    void Log::Init(const char* pattern) {
        try {
            // core logger (engine)
            spdlog::set_pattern(pattern);
            s_CoreLogger = spdlog::stdout_color_mt("RAYENGINE");
            s_CoreLogger->set_level(spdlog::level::trace);

            // client logger (sandbox / user)
            s_ClientLogger = spdlog::stdout_color_mt("APP");
            s_ClientLogger->set_level(spdlog::level::trace);

            // optional: flush on error or every tick
            spdlog::flush_on(spdlog::level::err);
        }
        catch (const spdlog::spdlog_ex& ex) {
            // fallback to minimal output on error
            printf("Log init failed: %s\n", ex.what());
        }
    }

    std::shared_ptr<spdlog::logger>& Log::GetCoreLogger() { return s_CoreLogger; }
    std::shared_ptr<spdlog::logger>& Log::GetClientLogger() { return s_ClientLogger; }

} 
