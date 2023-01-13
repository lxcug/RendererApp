// @author: lx
// @time: 2023/1/13
// @brief: 

#ifndef RENDERER_OPENGLWINDOW_H
#define RENDERER_OPENGLWINDOW_H

#include "Core/Window.h"
#include "Buffers/GraphicsContex.h"
#include "Platform/OpenGL/OpenGLContext.h"


namespace RendererSpace {
    class OpenGLWindow: public Window{
    public:
        OpenGLWindow(WindowProps& props);

        unsigned int getWidth() const override {
            return m_windowData.Width;
        }

        unsigned int getHeight() const override {
            return m_windowData.Height;
        }

        const std::string & getWindowName() const override {
            return m_windowData.Title;
        }

        void onUpdate() override;

        bool isVSync() const override {
            return m_windowData.bVSync;
        }

        void setVSync(bool enable) override final {
            m_windowData.bVSync = true;
        }


    private:
        GLFWwindow* m_window;  // glfw window

        struct WindowData {
            std::string Title;
            unsigned Width, Height;
            bool bVSync;
        };

        WindowData m_windowData;

        Scope<GraphicsContext> m_context;

        static bool s_glfwInit;
    };
}


#endif //RENDERER_OPENGLWINDOW_H
