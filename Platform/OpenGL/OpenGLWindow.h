// @author: lx
// @time: 2023/1/13
// @brief: 

#ifndef RENDERER_OPENGLWINDOW_H
#define RENDERER_OPENGLWINDOW_H

#include "Core/Window.h"
#include "GLFW/glfw3.h"
#include "Buffers/GraphicsContex.h"
#include "Platform/OpenGL/OpenGLContext.h"


namespace RendererSpace {
    class OpenGLWindow: public Window {
    public:
        OpenGLWindow(const WindowProps& props);
        ~OpenGLWindow() { shutDown(); }

        unsigned int getWidth() const override { return m_windowData.Width; }

        unsigned int getHeight() const override { return m_windowData.Height; }

        const std::string & getWindowName() const override {
            return m_windowData.Title;
        }

        void onUpdate() override;

        bool isVSync() const override {
            return m_windowData.VSync;
        }

        void setVSync(bool enable) override final;

        void* getNativeWindow() const override {
            return m_window;
        }

        void shutDown() override { glfwDestroyWindow(m_window); }

        void setEventCallback(const RendererSpace::Window::EventCallbackFun &callback) override {
            m_windowData.eventCallback = callback;
        }


        /**
         * @brief: For ImGui
         */
        void createImGuiContext() override;
        void destroyImGuiContext() override;

        /**
         * @brief: For OpenGL Events
         */
        void setGLFWWindowEventCallback();
        void setGLFWMouseEventCallback();
        void setGLFWKeyEventCallback();
        void setGLFWApplicationEventCallback();

        void setDarkTheme();


    private:
        GLFWwindow* m_window;  // glfw window

        struct WindowData {
            std::string Title;
            unsigned Width, Height;
            bool VSync;

            EventCallbackFun eventCallback;
        };

        WindowData m_windowData;

        Scope<GraphicsContext> m_context;

        static bool s_glfwInit;
    };
}


#endif //RENDERER_OPENGLWINDOW_H
