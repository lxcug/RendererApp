// @author: lx
// @time: 2022/10/15
// @brief: 

#pragma once
#ifndef GAME_OPENGLBUFFER_H
#define GAME_OPENGLBUFFER_H

#include "Core/Core.h"
#include "Buffers/Buffer.h"
#include "Glad/glad.h"


namespace RendererSpace {
    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer() = default;
        OpenGLVertexBuffer(unsigned size);  // NOLINT
        OpenGLVertexBuffer(float* vertices, unsigned size);

//        OpenGLVertexBuffer();
        ~OpenGLVertexBuffer() override;

        void bind() const override;
        void unBind() const override;

        void setData(const void *data, unsigned int size) override {
            glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
        }

        [[nodiscard]] const BufferLayout & getLayout() const override {
            return m_layout;
        }
        void setLayout(const BufferLayout &layout) override {
            m_layout = layout;
        }

    private:
        unsigned m_rendererID;
        BufferLayout m_layout;
    };


    class OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer() = default;
        OpenGLIndexBuffer(unsigned* indices, unsigned count);
        ~OpenGLIndexBuffer() override;

        void bind() const override;
        void unBind() const override;

        [[nodiscard]] unsigned int getCount() const override {
            return m_count;
        }

    private:
        unsigned m_rendererID;
        unsigned m_count = 0;
    };
}


#endif //GAME_OPENGLBUFFER_H
