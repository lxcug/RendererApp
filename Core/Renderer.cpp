#include "GLFW/glfw3.h"
#include "Renderer.h"
#include "OBJ_Loader.h"
#include "glm/ext/matrix_clip_space.hpp"


namespace RendererSpace {
    RendererAPI Renderer::s_rendererAPI = RendererAPI::OpenGL;
    Renderer3DData Renderer::s_data;

    void Renderer::loadOBJModel(const std::string &filepath) {
        objl::Loader Loader;
        bool loadOut = Loader.LoadFile(filepath);
        if(!loadOut) {
            ASSERT(false, "Can't load obj file!");
            return;
        }

        for(auto mesh: Loader.LoadedMeshes)
            s_data.CurrTriVertex = mesh.Vertices.size();

        s_data.VertexBuffer = RendererSpace::VertexBuffer::createVertexBuffer(s_data.CurrTriVertex * sizeof(RendererSpace::TriVertex));

        s_data.TriVertexBase = new TriVertex[s_data.CurrTriVertex];
        s_data.TriVertexPtr = s_data.TriVertexBase;

        std::vector<int> indices;
        for(auto& mesh: Loader.LoadedMeshes) {
            LOG_INFO("Mesh name: {}", mesh.MeshName);
            for(int i = 0; i < mesh.Vertices.size(); i++) {
                s_data.TriVertexPtr->Position = {mesh.Vertices[i].Position.X, mesh.Vertices[i].Position.Y,
                                                 mesh.Vertices[i].Position.Z};
                s_data.TriVertexPtr->TexCoord = {mesh.Vertices[i].TextureCoordinate.X,
                                                 mesh.Vertices[i].TextureCoordinate.Y};
                s_data.TriVertexPtr->Color = {.65, .213, .9, .5};
                s_data.TriVertexPtr->TexIndex = 0;
                s_data.TriVertexPtr->TilingFactor = 1.f;
                s_data.TriVertexPtr->EntityID = 0;
                s_data.TriVertexPtr++;
//                t->setNormal(j,Vector3f(mesh.Vertices[i+j].Normal.X,mesh.Vertices[i+j].Normal.Y,mesh.Vertices[i+j].Normal.Z));
            }

            for (int j = 0; j < mesh.Indices.size(); j += 3) {
                indices.push_back(mesh.Indices[j]);
                indices.push_back(mesh.Indices[j + 1]);
                indices.push_back(mesh.Indices[j + 2]);
            }
        }

        auto dataSize = (uint32_t)((uint8_t*)s_data.TriVertexPtr - (uint8_t*)s_data.TriVertexBase);
        s_data.TriVertexPtr = s_data.TriVertexBase;  // TODO: reset
        s_data.VertexBuffer->setData(s_data.TriVertexBase, dataSize);

        s_data.IndexCount = indices.size();
        auto* temp = new unsigned int [indices.size()];
        memcpy(temp, &indices[0], indices.size()*sizeof(unsigned int));
        Ref<IndexBuffer> triIndexBuffer = RendererSpace::IndexBuffer::createIndexBuffer(temp, indices.size());
        delete[] temp;


        s_data.VertexArray = RendererSpace::VertexArray::createVertexArray();
        s_data.VertexBuffer->setLayout({
                                               {ShaderDataType::Float3, "a_Position"},
                                               {ShaderDataType::Float4, "a_Color"},
                                               {ShaderDataType::Float2, "a_TexCoord"},
                                               {ShaderDataType::Float, "a_TexIndex"},
                                               {ShaderDataType::Float, "a_TilingFactor"},
                                               {ShaderDataType::Int, "a_EntityID"},
                                       });
        s_data.VertexArray->addVertexBuffer(s_data.VertexBuffer);
        s_data.VertexArray->setIndexBuffer(triIndexBuffer);

        s_data.Shader = RendererSpace::Shader::createShader("../Assets/Shaders/Renderer2D_Triangle.glsl");
        s_data.CameraUniformBuffer = UniformBuffer::createUniformBuffer(sizeof(CameraData), 0);
    }

    void Renderer::init() {
        loadOBJModel("../Assets/Models/spot/spot_triangulated_good.obj");
    }

    void Renderer::drawModel() {
        s_data.Shader->bind();
//        RenderCommand::drawIndexed(s_data.VertexArray, s_data.IndexCount);
        s_data.VertexArray->bind();
        glDrawElements(GL_TRIANGLES, (GLsizei)s_data.IndexCount, GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::beginScene() {
        s_data.CameraBuffer.ViewProjection = glm::perspective(glm::radians(90.f), 1.66f, .3f, 1000.f);
        s_data.CameraUniformBuffer->setData(&s_data.CameraBuffer, sizeof(CameraData));
    }

    void Renderer::beginScene(RendererCamera &camera) {
        s_data.CameraBuffer.ViewProjection = camera.getViewProjection();
        s_data.CameraUniformBuffer->setData(&s_data.CameraBuffer, sizeof(CameraData));
    }

    void Renderer::endScene() {

    }

    void Renderer::beginScene(Ref<RendererCamera> camera) {
        s_data.CameraBuffer.ViewProjection = camera->getViewProjection();
        s_data.CameraUniformBuffer->setData(&s_data.CameraBuffer, sizeof(CameraData));
    }


}
