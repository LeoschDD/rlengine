#pragma once

#include "core/Headers.hpp"

namespace rle
{
    class Log
    {
    private:
        static std::shared_ptr<spdlog::logger> core_logger_;
        static std::shared_ptr<spdlog::logger> client_logger_;

    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {return core_logger_;}
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {return client_logger_;}
    };
}

#define RLE_CORE_TRACE(...) ::rle::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RLE_CORE_INFO(...) ::rle::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RLE_CORE_WARN(...) ::rle::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RLE_CORE_ERROR(...) ::rle::Log::GetCoreLogger()->error(__VA_ARGS__)

#define RLE_TRACE(...) ::rle::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RLE_INFO(...) ::rle::Log::GetClientLogger()->info(__VA_ARGS__)
#define RLE_WARN(...) ::rle::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RLE_ERROR(...) ::rle::Log::GetClientLogger()->error(__VA_ARGS__)