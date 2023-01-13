// @author: lx
// @time: 2023/1/13
// @brief: 

#ifndef RENDERER_RENDERERAPP_H
#define RENDERER_RENDERERAPP_H

#include "Core.h"
#include "Logger.h"
#include "Window.h"


namespace RendererSpace {
    class RendererApp {
    public:
        RendererApp();
        ~RendererApp();

        void run();

        Window* getWindow() {
            return m_window.get();
        }

        /**
         * @brief return singleton instance of Application
         * @return the pointer of singleton instance of Application
         */
        static RendererApp* getApplication() {
            return s_instance;
        }

        virtual void close() {
            bStop = true;
        }

        virtual void beginImGui() const;
        virtual void endImGui() const;
        virtual void createImGuiContext();
        virtual void destroyImGuiContext();

    private:
        static RendererApp* s_instance;
        Scope<Window> m_window;

        bool bStop = false;
    };
}


#endif //RENDERER_RENDERERAPP_H
