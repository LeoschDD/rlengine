#include "core/Log.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> rle::Log::core_logger_;
std::shared_ptr<spdlog::logger> rle::Log::client_logger_;

void rle::Log::Init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    core_logger_ = spdlog::stdout_color_mt("RLENGINE");
    core_logger_->set_level(spdlog::level::trace);

    client_logger_ = spdlog::stdout_color_mt("APP");
    client_logger_->set_level(spdlog::level::trace);
    
    RLE_CORE_TRACE("initialized log");
}
