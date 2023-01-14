#include "Texture.h"
#include "Core/Logger.h"
#include "Core/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"


RendererSpace::Ref<RendererSpace::Texture2D> RendererSpace::Texture2D::createTexture2D(const std::string &path) {
    switch (RendererSpace::Renderer::getRendererAPI()) {
        case RendererAPI::None:
            ASSERT(false, "RendererAPI::None is currently not supported");
            return nullptr;
        case RendererAPI::OpenGL:
            return createRef<OpenGLTexture2D>(path);
    }
    return nullptr;
}

RendererSpace::Ref<RendererSpace::Texture2D> RendererSpace::Texture2D::createTexture2D(uint32_t width, uint32_t height) {
    switch (RendererSpace::Renderer::getRendererAPI()) {
        case RendererAPI::None:
            ASSERT(false, "RendererAPI::None is currently not supported");
            return nullptr;
        case RendererAPI::OpenGL:
            return createRef<OpenGLTexture2D>(width, height);
    }
    return nullptr;
}
