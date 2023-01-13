#include "GraphicsContex.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Core/Renderer.h"


namespace RendererSpace {

    Scope<GraphicsContext> GraphicsContext::createGraphicsContext(void *window) {
        switch (Renderer::getRendererAPI()) {
            case RendererAPI::None:
                ASSERT(false, "RendererAPI None is currently not supported");
            case RendererAPI::OpenGL:
                return createScope<OpenGLContext>((GLFWwindow*)window);
            case RendererAPI::VulKan:
                ASSERT(false, "RendererAPI None is currently not supported");
        }
        ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }
}
