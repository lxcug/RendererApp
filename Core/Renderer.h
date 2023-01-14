// @author: lx
// @time: 2023/1/13
// @brief: 

#ifndef RENDERER_RENDERER_H
#define RENDERER_RENDERER_H

#include "Core.h"
#include "Logger.h"
#include "glm.hpp"
#include "Buffers/Buffer.h"
#include "Buffers/VertexArray.h"
#include "Buffers/Shader.h"
#include "Buffers/UniformBuffer.h"
#include "RendererCamera.h"
#include "Buffers/Texture.h"


namespace RendererSpace {
    struct TriVertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;

        TriVertex(glm::vec3 position, glm::vec3 normal, glm::vec4 color, glm::vec2 texCoord, float texIndex): Position(position),
        Normal(normal), Color(color), TexCoord(texCoord), TexIndex(texIndex) {}
    };

    struct Renderer3DData {
        Ref<Shader> Shader;
        Ref<VertexBuffer> VertexBuffer;
        Ref<IndexBuffer> IndexBuffer;
        Ref<VertexArray> VertexArray;

        std::array<Ref<Texture2D>, 32> Textures;

        std::vector<TriVertex> Vertices;

        int CurrTriVertex = 0;
        int IndexCount = 0;
    };

    struct RendererStat {
        int VertexCount = 0;
        int IndexCount = 0;
        int TriangleCount = 0;
    };
}

namespace RendererSpace {
    enum class RendererAPI {
        None = 0x0,
        OpenGL,
    };

    class Renderer {
    public:
        static void init();

        static RendererAPI getRendererAPI() { return s_rendererAPI; }

        static void loadOBJModel(const std::string& filepath);

        static void beginScene();
        static void beginScene(RendererCamera& camera);
        static void beginScene(Ref<RendererCamera> camera);
        static void endScene();

        static void drawModel();

        static RendererStat& getStat() { return s_stat; }

//    private:
        static RendererAPI s_rendererAPI;
        static Renderer3DData s_data;
        static RendererStat s_stat;
    };
}


#endif //RENDERER_RENDERER_H
