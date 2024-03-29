// @author: lx
// @time: 2022/10/15
// @brief: 

#pragma once
#ifndef GAME_OPENGLCONTEXT_H
#define GAME_OPENGLCONTEXT_H

#include "Core/Core.h"
#include "Core/Logger.h"
#include "Buffers/GraphicsContex.h"


class GLFWwindow;
namespace RendererSpace {
    class OpenGLContext : public GraphicsContext {
    public:
        explicit OpenGLContext(GLFWwindow* windowHandle);

        void init() override;
        void swapBuffers() override;

    private:
        GLFWwindow* m_windowHandle;
    };
}


#endif //GAME_OPENGLCONTEXT_H
