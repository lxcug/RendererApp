#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"


RendererSpace::Ref<spdlog::logger> RendererSpace::Logger::s_logger;

void RendererSpace::Logger::init() {
    // set log format
    spdlog::set_pattern("%^[%T] %n: %v%$");

    RendererSpace::Logger::s_logger = spdlog::stdout_color_mt("Renderer");
   RendererSpace::Logger::s_logger->set_level(spdlog::level::trace);
}
