// @author: lx
// @time: 2023/1/13
// @brief: 

#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H

#include "Core.h"
#include "Logger.h"


namespace RendererSpace {
    enum class RendererAPI {
        None = 0x0,
        OpenGL,
        VulKan,
    };

    class Renderer {
    public:
        static void init() {

        }

        static RendererAPI getRendererAPI() {
            return s_rendererAPI;
        }

    private:
        static RendererAPI s_rendererAPI;
    };
}


#endif //RENDERER_RENDERER_H
