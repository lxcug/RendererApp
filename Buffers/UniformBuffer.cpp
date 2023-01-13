#include "UniformBuffer.h"
#include "Core/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"


RendererSpace::Ref<RendererSpace::UniformBuffer> RendererSpace::UniformBuffer::createUniformBuffer(uint32_t size, uint32_t binding) {
    switch (RendererSpace::Renderer::getRendererAPI()) {
        case RendererAPI::None: {
            ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        }
        case RendererAPI::OpenGL: {
            return RendererSpace::createRef<OpenGLUniformBuffer>(size, binding);
        }
    }
    ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
