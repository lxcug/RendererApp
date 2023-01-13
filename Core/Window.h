// @author: lx
// @time: 2023/1/13
// @brief: 

#ifndef RENDERER_WINDOW_H
#define RENDERER_WINDOW_H

#include "Core.h"
#include "Logger.h"
#include "Events/Event.h"


namespace RendererSpace {
    struct WindowProps {
        explicit WindowProps(std::string title="Unnamed Window", unsigned width=1600, unsigned height=900):
                Title(std::move(title)), Width(width), Height(height) {}

        std::string Title;
        unsigned Width;
        unsigned Height;
    };

    class Window {
    public:
        virtual ~Window() = default;  // 析构函数不能被声明为纯虚函数, 因为会被派生类调用

        virtual void onUpdate() = 0;

        virtual unsigned getWidth() const = 0;
        virtual unsigned getHeight() const = 0;
        virtual const std::string& getWindowName() const = 0;

        // 垂直同步 Vertical Synchronization
        virtual void setVSync(bool enable) = 0;
        virtual bool isVSync() const = 0;

        virtual void* getNativeWindow() const = 0;

        virtual void shutDown() = 0;

        using EventCallbackFun = std::function<void(Event&)>;
        virtual void setEventCallback(const EventCallbackFun& callback) = 0;

        static Scope<Window> createWindow(const WindowProps& props = WindowProps());

    private:
        void* window;
    };
}



#endif //RENDERER_WINDOW_H
