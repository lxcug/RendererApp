#include "OpenGLTexture.h"
#include "Glad/glad.h"
#include "Core/Logger.h"


RendererSpace::OpenGLTexture2D::OpenGLTexture2D(std::string path): m_path(std::move(path)) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);

    stbi_uc* data = nullptr;
    data = stbi_load(m_path.c_str(), &width, &height, &channels, 0);

    if(data) {
        m_isLoaded = true;
        m_width = width;
        m_height = height;
        if(channels == 4) {
            m_internalFormat = GL_RGBA8;
            m_dataFormat = GL_RGBA;
        }
        else if(channels == 3) {
            m_internalFormat = GL_RGB8;
            m_dataFormat = GL_RGB;
        }

        ASSERT(m_internalFormat & m_dataFormat, "Format not supported!");

        glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);

        glTextureStorage2D(m_rendererID, 8, m_internalFormat, m_width, m_height);

        glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }
    else {
        LOG_WARN("Failed to load Texture!");
    }
}

RendererSpace::OpenGLTexture2D::OpenGLTexture2D(unsigned int width, unsigned int height): m_width(width), m_height(height) {
    m_isLoaded = true;

    m_internalFormat = GL_RGBA8;
    m_dataFormat = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
    glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

    glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

RendererSpace::OpenGLTexture2D::~OpenGLTexture2D() {
    glDeleteTextures(1, &m_rendererID);
}

void RendererSpace::OpenGLTexture2D::bind(unsigned int slot) {
    glBindTextureUnit(slot, m_rendererID);
}

void RendererSpace::OpenGLTexture2D::setData(void *data, uint32_t size) {
    uint32_t bpp = m_dataFormat == GL_RGBA ? 4 : 3;
    ASSERT(size == m_width * m_height * bpp, "Data must be entire texture!");
    glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_dataFormat, GL_UNSIGNED_BYTE, data);
}

void RendererSpace::OpenGLTexture2D::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}
