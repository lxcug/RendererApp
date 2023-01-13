// @author: lx
// @time: 2023/1/13
// @brief: 

#ifndef RENDERER_LOGGER_H
#define RENDERER_LOGGER_H

#include "Core.h"
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)


namespace RendererSpace {
    class Logger {
    public:
        static void init();


        static Ref<spdlog::logger>& getLogger() {
            return s_logger;
        }

    private:
        static Ref<spdlog::logger> s_logger;
    };
}


#define LOG_INFO(...)    ::RendererSpace::Logger::getLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...)   ::RendererSpace::Logger::getLogger()->trace(__VA_ARGS__)
#define LOG_WARN(...)    ::RendererSpace::Logger::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)   ::RendererSpace::Logger::getLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...)   ::RendererSpace::Logger::getLogger()->critical(__VA_ARGS__)


#endif //RENDERER_LOGGER_H
