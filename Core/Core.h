// @author: lx
// @time: 2023/1/13
// @brief: 

#ifndef RENDERER_CORE_H
#define RENDERER_CORE_H

#define ASSERT(x, ...) if(!(x)) { LOG_FATAL("Assertion Failed: {0}", __VA_ARGS__); raise(SIGABRT); }

#include "iostream"
#define BIND_EVENT_FUN(fun) std::bind(&fun, this, std::placeholders::_1)

#include <memory>
namespace RendererSpace {
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> createScope(Args&& ... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> createRef(Args&& ... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}


#endif //RENDERER_CORE_H
