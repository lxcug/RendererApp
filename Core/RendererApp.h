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
#include "glm.hpp"
#include "Renderer.h"
#include "Core/RendererCamera.h"
#include "Buffers/FrameBuffer.h"
#include "Core/Mesh.h"


namespace RendererSpace {
    class RendererApp {
    public:
        RendererApp();
        ~RendererApp();

        void run();
        void onEvent(Event& event);
        void onUpdate();
        void onImGuiRender();

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
        void statImGuiRender();


    private:
        static RendererApp* s_instance;
        Scope<Window> m_window;

        bool b_stop = false;
        Ref<RendererCamera> m_rendererCamera;

        // For viewport
        glm::vec2 m_viewportSize = {0.f, 0.f};
        glm::vec2 m_viewportBounds[2];
        Ref<FrameBuffer> m_frameBuffer;
        bool b_viewportFocused = false, b_viewportHovered = false, b_blockEvent = false;

//        Mesh* m_mesh;
    };
}


#endif //RENDERER_RENDERERAPP_H
