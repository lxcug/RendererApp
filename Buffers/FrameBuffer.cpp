#include "FrameBuffer.h"
#include "Core/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"


RendererSpace::Ref<RendererSpace::FrameBuffer> RendererSpace::FrameBuffer::createFrameBuffer(const RendererSpace::FrameBufferSpecification &spec) {
    switch (RendererSpace::Renderer::getRendererAPI()) {
        case RendererAPI::None:
            ASSERT(false, "RendererAPI::None is currently not supported");
            return nullptr;
        case RendererAPI::OpenGL:
            return createRef<OpenGLFrameBuffer>(spec);
    }
    ASSERT(false, "Unknown RendererAPI");
    return nullptr;
}
