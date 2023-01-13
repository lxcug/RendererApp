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


namespace RendererSpace {
    struct TriVertex {
        glm::vec3 Position;
        glm::vec4 Color = {100, 100, 100, 0.5};
        glm::vec2 TexCoord;
        float TexIndex = 0;
        float TilingFactor = 1.f;
        int EntityID = 0;
    };

    struct CameraData {
        glm::mat4 ViewProjection;
    };

    struct Renderer3DData {
        Ref<Shader> Shader;
        Ref<VertexBuffer> VertexBuffer;
        Ref<IndexBuffer> IndexBuffer;
        Ref<VertexArray> VertexArray;

        CameraData CameraBuffer;
        Ref<UniformBuffer> CameraUniformBuffer;


        TriVertex* TriVertexBase = nullptr;
        TriVertex* TriVertexPtr = nullptr;

        int CurrTriVertex = 0;
        int IndexCount = 0;
    };
}

namespace RendererSpace {
    enum class RendererAPI {
        None = 0x0,
        OpenGL,
        VulKan,
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

    private:
        static RendererAPI s_rendererAPI;
        static Renderer3DData s_data;
    };
}


#endif //RENDERER_RENDERER_H
