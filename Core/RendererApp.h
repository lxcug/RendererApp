// @author: lx
// @time: 2023/1/13
// @brief: 

#ifndef RENDERER_RENDERERAPP_H
#define RENDERER_RENDERERAPP_H

#include "Core.h"
#include "Logger.h"
#include "Window.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"


namespace RendererSpace {
    class RendererApp {
    public:
        RendererApp();
        ~RendererApp();

        void run();
        void onEvent(Event& event);
        void onUpdate();
        void onImGui();

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

        void beginImGuiFrame() const;
        void endImGuiFrame() const;

    private:
        bool onWindowClose(WindowCloseEvent &event);
        bool onWindowResize(WindowResizeEvent &event);
        void enableImGuiDocking();


    private:
        static RendererApp* s_instance;
        Scope<Window> m_window;

        bool bStop = false;
    };
}


#endif //RENDERER_RENDERERAPP_H
