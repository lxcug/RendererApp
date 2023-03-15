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
#include "Mesh.h"
#include "Components/Components.h"
#include "Object.h"
#include "GUID.h"


namespace RendererSpace {
    struct RendererStat {
        int VertexCount = 0;
        int IndexCount = 0;
        int TriangleCount = 0;

        void resetStat() {
            VertexCount = 0;
            IndexCount = 0;
            TriangleCount = 0;
        }
    };


    struct PointLight {
        glm::vec3 Ka, Kd, Ks;
        glm::vec3 Pos;
        glm::vec3 Intensity;

        RendererSpace::TransformComponent TransformComponent;

        PointLight(): Ka(.2, .2, .2), Kd(.1, .1, .1), Ks(0.3, 0.3, 0.3),
        Pos(0, 0, 0), Intensity(20., 20., 20.) {}

        glm::mat4 getTransform() {
            return TransformComponent.getTransform();
        }

        glm::vec3 getTransformedPosition() {
            auto res = getTransform() * glm::vec4(Pos, 1.);
            return {res.x, res.y, res.z};
        }
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

        static void placePointLight(PointLight& pointLight);
        static Ref<PointLight> getPointLight() {
            return s_pointLight;
        }
        static void updatePointLight(PointLight& pointLight);

        static void beginScene();
//        static void beginScene(RendererCamera& camera);
        static void beginScene(Ref<RendererCamera> camera);
        static void endScene();
        static void drawModel(const glm::mat4& transform = glm::identity<glm::mat4>());

        static void drawModel(Ref<Object> object, const glm::mat4& transform);
        static void drawModel(Object& object, const glm::mat4& transform);

        void meshSubdivision() {

        }
        void meshSimplification() {

        }
        void meshRegularization() {

        }

        static RendererStat& getStat() { return s_stat; }

        static Ref<PointLight> s_pointLight;

    private:
        static RendererAPI s_rendererAPI;
        static RendererStat s_stat;
        static Ref<Object> s_object;
        static Ref<Shader> s_shader;
        static std::array<Ref<Texture2D>, 32> s_textures;
    };
}


#endif //RENDERER_RENDERER_H
