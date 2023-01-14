#include "GLFW/glfw3.h"
#include "Mesh.h"


RendererSpace::Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices) {
    setUpMesh(vertices, indices);
}

void RendererSpace::Mesh::setUpMesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices) {
    m_vertexBuffer = VertexBuffer::createVertexBuffer(vertices.size() * sizeof(Vertex));
    m_vertexBuffer->setData(vertices.data(), vertices.size() * sizeof(Vertex));
    m_vertexBuffer->setLayout({
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float3, "a_Normal"},
        {ShaderDataType::Float2, "a_TexCoord"},
    });

    m_indexBuffer = IndexBuffer::createIndexBuffer(indices.data(), indices.size());

    m_vertexArray = VertexArray::createVertexArray();
    m_vertexArray->addVertexBuffer(m_vertexBuffer);
    m_vertexArray->setIndexBuffer(m_indexBuffer);

    m_shader = Shader::createShader("../Assets/Shaders/Renderer2D_Triangle.glsl");
    m_shader->bind();

//    m_texture = Texture2D::createTexture2D("../Assets/Models/spot/spot_texture.png");
//    m_texture->bind(0);
//    m_shader->setInt("u_Texture", 0);
}

void RendererSpace::Mesh::draw() {
    glDrawElements(GL_TRIANGLES, (GLsizei)m_vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
}
