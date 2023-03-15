// @author: lx
// @time: 2023/1/14
// @brief: 

#ifndef RENDERER_MESH_H
#define RENDERER_MESH_H

#include "Core.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"
#include <iostream>
#include "Buffers/Shader.h"
#include "Buffers/Buffer.h"
#include "Buffers/VertexArray.h"
#include "Buffers/Texture.h"


namespace RendererSpace {
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoord;
        float TexIndex;
        int EntityId;

        Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord, float texIndex, int entityId):
        Position(position), Normal(normal), TexCoord(texCoord), TexIndex(texIndex), EntityId(entityId) {}
    };

    template<class VertexType>
    class Mesh {
    public:
        Mesh(std::vector<Vertex>& vertices, std::vector<unsigned>& indices) {
            setUpMesh(vertices, indices);
        }

        void setUpMesh(std::vector<Vertex>& vertices, std::vector<unsigned>& indices) {
            m_vertexBuffer = VertexBuffer::createVertexBuffer(vertices.size() * sizeof(Vertex));
            m_vertexBuffer->setData(vertices.data(), vertices.size() * sizeof(Vertex));
            m_vertexBuffer->setLayout({
                                              {ShaderDataType::Float3, "a_Position"},
                                              {ShaderDataType::Float3, "a_Normal"},
                                              {ShaderDataType::Float2, "a_TexCoord"},
                                              {ShaderDataType::Float, "a_TexIndex"},
                                              {ShaderDataType::Int, "a_EntityId"}
                                      });
            m_indexBuffer = IndexBuffer::createIndexBuffer(indices.data(), indices.size());

            m_vertexArray = VertexArray::createVertexArray();
            m_vertexArray->addVertexBuffer(m_vertexBuffer);
            m_vertexArray->setIndexBuffer(m_indexBuffer);
            m_vertexArray->unbind();
        }

        void draw() {
            m_vertexArray->bind();
            glDrawElements(GL_TRIANGLES, (GLsizei)getIndexCount(),
                           GL_UNSIGNED_INT, nullptr);
        }

        int getVertexCount() const {
            return m_vertexBuffer->getVertexDataSize() / sizeof(VertexType);
        }
        int getIndexCount() const {
            return m_indexBuffer->getCount();
        }

    private:
        Ref<VertexBuffer> m_vertexBuffer;
        Ref<IndexBuffer> m_indexBuffer;
        Ref<VertexArray> m_vertexArray;
        Ref<Texture2D> m_texture;
    };
}


#endif //RENDERER_MESH_H
