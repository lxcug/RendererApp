#include "VertexArray.h"
#include "Core/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"


RendererSpace::Ref<RendererSpace::VertexArray> RendererSpace::VertexArray::createVertexArray() {
    switch (Renderer::getRendererAPI()) {
        case RendererAPI::None:
            ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::OpenGL:
            return createRef<RendererSpace::OpenGLVertexArray>();
    }
    return nullptr;
}
