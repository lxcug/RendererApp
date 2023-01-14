// @author: lx
// @time: 2022/10/26
// @brief: 

#pragma once
#ifndef GAME_OPENGLFRAMEBUFFER_H
#define GAME_OPENGLFRAMEBUFFER_H

#include "Core/Core.h"
#include "Buffers//FrameBuffer.h"
#include "Core/Logger.h"


namespace RendererSpace {
    class OpenGLFrameBuffer : public FrameBuffer {
    public:
        OpenGLFrameBuffer(FrameBufferSpecification spec);
        ~OpenGLFrameBuffer();

        void bind() override;
        void unbind() override;

        void resize(unsigned int width, unsigned int height) override;

        int readPixel(uint32_t attachmentIndex, int x, int y) override;

        void clearAttachment(uint32_t attachmentIndex, int value) override;

        void invalidate();

        [[nodiscard]] const FrameBufferSpecification& getSpecification() const override {
            return m_specification;
        }
        [[nodiscard]] unsigned int getColorAttachmentRendererID(unsigned index = 0) const override {
            ASSERT(index < m_colorAttachments.size(), "");
            return m_colorAttachments[index];
        }

    private:
        unsigned m_rendererID = 0;

        FrameBufferSpecification m_specification;

        std::vector<FrameBufferTextureSpecification> m_colorAttachmentSpecifications;
        FrameBufferTextureSpecification m_depthAttachmentSpecification = FrameBufferTextureFormat::None;

        std::vector<uint32_t> m_colorAttachments;
        uint32_t m_depthAttachment = 0;
    };
}


#endif //GAME_OPENGLFRAMEBUFFER_H
