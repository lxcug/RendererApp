// @author: lx
// @time: 2023/1/14
// @brief: 

#ifndef RENDERER_MESH_H
#define RENDERER_MESH_H

#include "Core.h"
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

        Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord): Position(position), Normal(normal), TexCoord(texCoord) {}
    };

    class Mesh {
    public:
        Mesh(std::vector<Vertex>& vertices, std::vector<unsigned>& indices);
        void setUpMesh(std::vector<Vertex>& vertices, std::vector<unsigned>& indices);
        void draw();


//    private:
        Ref<VertexBuffer> m_vertexBuffer;
        Ref<IndexBuffer> m_indexBuffer;
        Ref<VertexArray> m_vertexArray;
        Ref<Shader> m_shader;
        Ref<Texture2D> m_texture;
    };
}


#endif //RENDERER_MESH_H
