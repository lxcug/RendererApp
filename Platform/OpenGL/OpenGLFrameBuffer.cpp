#include "OpenGLFrameBuffer.h"
#include <utility>
#include "Glad/glad.h"


namespace RendererSpace {
    static const uint32_t s_MaxFramebufferSize = 8192;

    namespace Utils {

        static GLenum rendererFrameBufferTextureFormatToGL(FrameBufferTextureFormat format) {
            switch (format) {
                case FrameBufferTextureFormat::RGBA8:       return GL_RGBA8;
                case FrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
            }
            ASSERT(false, "rendererFrameBufferTextureFormatToGL False");
            return 0;
        }

        static void attachColorTexture(uint32_t id, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index) {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, id, 0);
        }
    }
}


RendererSpace::OpenGLFrameBuffer::OpenGLFrameBuffer(RendererSpace::FrameBufferSpecification spec): m_specification(std::move(spec)) {
    for(auto specification : m_specification.Attachments.Attachments) {
        if(specification.TextureFormat == FrameBufferTextureFormat::DEPTH24STENCIL8)
            m_depthAttachmentSpecification = specification;
        else
            m_colorAttachmentSpecifications.emplace_back(specification);
    }
    invalidate();
}

void RendererSpace::OpenGLFrameBuffer::invalidate() {
    using namespace RendererSpace;

    if(m_rendererID) {
        glDeleteFramebuffers(1, &m_rendererID);
        glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
        glDeleteRenderbuffers(1, &m_depthAttachment);

        m_colorAttachments.clear();
        m_depthAttachment = 0;
    }

    glCreateFramebuffers(1, &m_rendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

    // Attachments
    if(m_colorAttachmentSpecifications.size()) {
        m_colorAttachments.resize(m_colorAttachmentSpecifications.size());
        glCreateTextures(GL_TEXTURE_2D, m_colorAttachments.size(), m_colorAttachments.data());

        for(size_t i = 0; i < m_colorAttachments.size(); i++) {
            glBindTexture(GL_TEXTURE_2D, m_colorAttachments[i]);

            switch (m_colorAttachmentSpecifications[i].TextureFormat) {
                case FrameBufferTextureFormat::RGBA8: {
                    Utils::attachColorTexture(m_colorAttachments[i], GL_RGBA8, GL_RGBA,
                                              m_specification.Width, m_specification.Height, i);
                    break;
                }
                case FrameBufferTextureFormat::RED_INTEGER: {
                    Utils::attachColorTexture(m_colorAttachments[i], GL_R32I, GL_RED_INTEGER,
                                              m_specification.Width, m_specification.Height, i);
                            break;
                }
            }
        }
    }

    if(m_depthAttachmentSpecification.TextureFormat != FrameBufferTextureFormat::None) {
        glGenRenderbuffers(1, &m_depthAttachment);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthAttachment);

        switch (m_depthAttachmentSpecification.TextureFormat) {
            case FrameBufferTextureFormat::DEPTH24STENCIL8:
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_specification.Width, m_specification.Height);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthAttachment);
                break;
        }
    }

    if(m_colorAttachments.size() > 1) {
        ASSERT(m_colorAttachments.size() <= 4, "m_colorAttachments.size() <= 4 False");
        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers(m_colorAttachments.size(), buffers);
    }
    else if(m_colorAttachments.empty()) {
        // Only depth-pass
        glDrawBuffer(GL_NONE);
    }
    ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

RendererSpace::OpenGLFrameBuffer::~OpenGLFrameBuffer() {
    glDeleteFramebuffers(1, &m_rendererID);
    glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
    glDeleteTextures(1, &m_depthAttachment);
}

void RendererSpace::OpenGLFrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
    glViewport(0, 0, m_specification.Width, m_specification.Height);
}

void RendererSpace::OpenGLFrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RendererSpace::OpenGLFrameBuffer::resize(unsigned int width, unsigned int height) {
    m_specification.Width = width;
    m_specification.Height = height;
    invalidate();
}

int RendererSpace::OpenGLFrameBuffer::readPixel(uint32_t attachmentIndex, int x, int y) {
    ASSERT(attachmentIndex < m_colorAttachments.size(), "attachmentIndex < m_colorAttachments.size() False");

    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    int pixelData;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
    return pixelData;
}

void RendererSpace::OpenGLFrameBuffer::clearAttachment(uint32_t attachmentIndex, int value) {
    ASSERT(attachmentIndex < m_colorAttachments.size(), "attachmentIndex < m_colorAttachments.size() False");

    auto& spec = m_colorAttachmentSpecifications[attachmentIndex];
    glClearTexImage(m_colorAttachments[attachmentIndex], 0,
                    Utils::rendererFrameBufferTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
}
