#include "Buffer.h"
#include "Core/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"


namespace RendererSpace {

    Ref<VertexBuffer> VertexBuffer::createVertexBuffer(unsigned int size) {
        switch (Renderer::getRendererAPI()) {
            case RendererAPI::None:
                ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::OpenGL:
                return createRef<OpenGLVertexBuffer>(size);
        }
        ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::createVertexBuffer(float *vertices, unsigned int size) {
        switch (Renderer::getRendererAPI()) {
            case RendererAPI::None:
                ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::OpenGL:
                return createRef<OpenGLVertexBuffer>(vertices, size);
        }
        ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }


    Ref<IndexBuffer> IndexBuffer::createIndexBuffer(unsigned int *indices, unsigned int count) {
        switch (Renderer::getRendererAPI()) {
            case RendererAPI::None:
                ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::OpenGL:
                return createRef<OpenGLIndexBuffer>(indices, count);
        }
        ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}