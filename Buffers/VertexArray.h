// @author: lx
// @time: 2022/10/15
// @brief: 

#pragma once
#ifndef GAME_VERTEXARRAY_H
#define GAME_VERTEXARRAY_H

#include "Core/Core.h"
#include <memory>
#include "Buffer.h"


namespace RendererSpace {
    class VertexArray {
    public:
        virtual ~VertexArray() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
        virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

        virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const = 0;
        virtual const Ref<IndexBuffer>& getIndexBuffer() const = 0;

        static Ref<VertexArray> createVertexArray();
    };
}


#endif //GAME_VERTEXARRAY_H
