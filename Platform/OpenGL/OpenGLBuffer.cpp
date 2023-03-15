#include "OpenGLBuffer.h"
#include "Glad/glad.h"

// --------------------------------------Vertex Buffer--------------------------------------

RendererSpace::OpenGLVertexBuffer::OpenGLVertexBuffer(unsigned int size) {
    glGenBuffers(1, &m_rendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);  // bind vertex buffer
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    m_dataSize = (int)size;
}

RendererSpace::OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, unsigned size) {  // NOLINT
    glGenBuffers(1, &m_rendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);  // bind vertex buffer
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    m_dataSize = (int)size;
}

RendererSpace::OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    glDeleteBuffers(1, &m_rendererID);
}

void RendererSpace::OpenGLVertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);  // bind OpenGL vertex buffer
}

void RendererSpace::OpenGLVertexBuffer::unBind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind OpenGL vertex buffer
}

void RendererSpace::OpenGLVertexBuffer::setData(const void *data, unsigned int size) {
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    m_dataSize = (int)size;
}


// --------------------------------------Index Buffer--------------------------------------

RendererSpace::OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int *indices, unsigned int count): m_count(count) {
    glGenBuffers(1, &m_rendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);  // bind index buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned), indices, GL_STATIC_DRAW);
}

RendererSpace::OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    glDeleteBuffers(1, &m_rendererID);
}

void RendererSpace::OpenGLIndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);  // bind OpenGL index buffer
}

void RendererSpace::OpenGLIndexBuffer::unBind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // unbind OpenGL index buffer
}
