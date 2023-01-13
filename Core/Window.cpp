#include "Window.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLWindow.h"


RendererSpace::Scope<RendererSpace::Window> RendererSpace::Window::createWindow(const WindowProps &props) {
    switch (Renderer::getRendererAPI()) {
        case RendererAPI::None:
            ASSERT(false, "RendererAPI None is currently not supported");
        case RendererAPI::OpenGL:
            return createScope<OpenGLWindow>(props);
        case RendererAPI::VulKan:
            ASSERT(false, "RendererAPI None is currently not supported");
    }
    ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}


