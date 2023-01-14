#include "GLFW/glfw3.h"
#include "Renderer.h"
#include "OBJ_Loader.h"
#include "Buffers/FrameBuffer.h"


namespace RendererSpace {
    RendererAPI Renderer::s_rendererAPI = RendererAPI::OpenGL;
    Renderer3DData Renderer::s_data;
    RendererStat Renderer::s_stat;

    void Renderer::loadOBJModel(const std::string &filepath) {
        objl::Loader Loader;
        bool loadOut = Loader.LoadFile(filepath);
        if (!loadOut) {
            ASSERT(false, "Can't load obj file!");
            return;
        }

        for (auto mesh: Loader.LoadedMeshes)
            s_data.CurrTriVertex += mesh.Vertices.size();

        s_data.VertexBuffer = RendererSpace::VertexBuffer::createVertexBuffer(
                s_data.CurrTriVertex * sizeof(RendererSpace::TriVertex));

        s_data.Vertices.reserve(s_data.CurrTriVertex);

        std::vector<int> indices;
        for (auto &mesh: Loader.LoadedMeshes) {
            LOG_INFO("Mesh name: {}", mesh.MeshName);
            for (int i = 0; i < mesh.Vertices.size(); i++) {
                s_data.Vertices.push_back(
                        {
                            glm::vec3(mesh.Vertices[i].Position.X, mesh.Vertices[i].Position.Y,mesh.Vertices[i].Position.Z),
                            glm::vec3(mesh.Vertices[i].Normal.X, mesh.Vertices[i].Normal.Y,mesh.Vertices[i].Normal.Z),
                            glm::vec4(169 / 255., 169 / 255., 169 / 255., 1.),
                            glm::vec2(mesh.Vertices[i].TextureCoordinate.X,mesh.Vertices[i].TextureCoordinate.Y),
                            0.
                        }
                        );
            }

            for (int j = 0; j < mesh.Indices.size(); j += 3) {
                indices.push_back(mesh.Indices[j]);
                indices.push_back(mesh.Indices[j + 1]);
                indices.push_back(mesh.Indices[j + 2]);
            }
        }

        s_data.VertexBuffer->setData(s_data.Vertices.data(), s_data.Vertices.size() * sizeof(TriVertex));

        s_data.IndexCount = indices.size();
        auto *temp = new unsigned int[indices.size()];
        memcpy(temp, &indices[0], indices.size() * sizeof(unsigned int));
        Ref<IndexBuffer> triIndexBuffer = RendererSpace::IndexBuffer::createIndexBuffer(temp, indices.size());

        delete[] temp;


        s_data.VertexArray = RendererSpace::VertexArray::createVertexArray();
        s_data.VertexBuffer->setLayout({
                                               {ShaderDataType::Float3, "a_Position"},
                                               {ShaderDataType::Float3, "a_Normal"},
                                               {ShaderDataType::Float4, "a_Color"},
                                               {ShaderDataType::Float2, "a_TexCoord"},
                                               {ShaderDataType::Float, "a_TexIndex"},
                                       });
        s_data.VertexArray->addVertexBuffer(s_data.VertexBuffer);
        s_data.VertexArray->setIndexBuffer(triIndexBuffer);

        s_data.Shader = RendererSpace::Shader::createShader("../Assets/Shaders/Renderer2D_Triangle.glsl");
        s_data.Shader->bind();

        s_data.Textures[0] = Texture2D::createTexture2D("../Assets/Models/spot/spot_texture.png");
        s_data.Textures[0]->bind(0);
        s_data.Textures[1] = Texture2D::createTexture2D("../Assets/Models/rock/rock.png");
        s_data.Textures[1]->bind(1);
        s_data.Textures[2] = Texture2D::createTexture2D("../Assets/Models/Crate/crate_1.jpg");
        s_data.Textures[2]->bind(2);


        // NOTE: Actually we don't need to do this, cause the initial value of uniform sample2D u_Textures[i] is i
        int samplers[32];
        for(int i = 0; i < 32; i++)
            samplers[i] = i;
        s_data.Shader->setIntArray("u_Textures", samplers, 32);



        // statistic
        s_stat.VertexCount = s_data.CurrTriVertex;
        s_stat.IndexCount = s_data.IndexCount;
        s_stat.TriangleCount = s_data.IndexCount / 3;
    }

    void Renderer::init() {
        loadOBJModel("../Assets/Models/spot/spot_triangulated_good.obj");
    }

    void Renderer::drawModel() {
        s_data.Shader->bind();
        s_data.VertexArray->bind();
        glDrawElements(GL_TRIANGLES, (GLsizei)s_data.IndexCount, GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::beginScene(RendererCamera &camera) {
        s_data.Shader->setMat4("u_ViewProjection", camera.getViewProjection());
    }

    void Renderer::endScene() {

    }

    void Renderer::beginScene(Ref<RendererCamera> camera) {
        s_data.Shader->setMat4("u_ViewProjection", camera->getViewProjection());
    }

    void Renderer::beginScene() {
        glm::mat4 viewProjection = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
        s_data.Shader->setMat4("u_ViewProjection", viewProjection);
    }
}
