#include "Glad/glad.h"
#include "Renderer.h"
#include "Buffers/FrameBuffer.h"


namespace RendererSpace {
    RendererAPI Renderer::s_rendererAPI = RendererAPI::OpenGL;
    RendererStat Renderer::s_stat;
    Ref<Object> Renderer::s_object;
    Ref<Shader> Renderer::s_shader;
    std::array<Ref<Texture2D>, 32> Renderer::s_textures;
    Ref<PointLight> Renderer::s_pointLight;

    void Renderer::init() {
        if(RendererSpace::GlobalSettings::b_enableBlend) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        if(RendererSpace::GlobalSettings::b_useDepthBuffer) {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
        }

        glActiveTexture(GL_ACTIVE_TEXTURE);

        s_shader = RendererSpace::Shader::createShader("../Assets/Shaders/TextureShader.glsl");
        s_shader->bind();

        // Place Point Light
        if(GlobalSettings::b_placePointLight) {
            s_pointLight = createRef<PointLight>();
            placePointLight(*s_pointLight);
        }

        s_textures[0] = Texture2D::createTexture2D("../Assets/Models/spot/spot_texture.png");
        s_textures[0]->bind(0);
        s_textures[1] = Texture2D::createTexture2D("../Assets/Models/rock/rock.png");
        s_textures[1]->bind(1);
        s_textures[2] = Texture2D::createTexture2D("../Assets/Models/Crate/crate_1.jpg");
        s_textures[2]->bind(2);
        // NOTE: Actually we don't need to do this, cause the initial value of uniform sample2D u_Textures[i] is i
        int samplers[32];
        for(int i = 0; i < 32; i++)
            samplers[i] = i;
        s_shader->setIntArray("u_Textures", samplers, 32);
    }

    void Renderer::loadOBJModel(const std::string &filepath) {
        s_stat.resetStat();

        s_object = createRef<Object>();
        s_object->loadObject(filepath);

        // statistic
        for(const auto& mesh: s_object->getMeshes()) {
            s_stat.VertexCount += (int)(mesh.getVertexCount());
            s_stat.IndexCount += (int)mesh.getIndexCount();
            s_stat.TriangleCount += s_stat.IndexCount/3;
        }
    }

    void Renderer::drawModel(Ref<Object> object, const glm::mat4& transform) {
        drawModel(*object, transform);
    }

    void Renderer::drawModel(Object& object, const glm::mat4 &transform) {
        if(GlobalSettings::b_wireframeMode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        if(GlobalSettings::b_placePointLight) {
            updatePointLight(*s_pointLight);
        }

        s_shader->setMat4("u_Model", transform);
        object.draw();
    }

    void Renderer::drawModel(const glm::mat4 &transform) {
        drawModel(s_object, transform);
    }

    void Renderer::endScene() {

    }

    void Renderer::beginScene(Ref<RendererCamera> camera) {
        if(GlobalSettings::b_placePointLight) {
            s_shader->setFloat3("u_EyePos", camera->getPosition());
        }
        s_shader->setMat4("u_View", camera->getView());
        s_shader->setMat4("u_Projection", camera->getProjection());
    }

    void Renderer::beginScene() {
//        glm::mat4 viewProjection = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
//        s_shader->setMat4("u_ViewProjection", viewProjection);
    }

    void Renderer::placePointLight(PointLight& pointLight) {
//        LOG_INFO("Set Point Light on ({}, {}, {})", pointLight.Pos.x, pointLight.Pos.y, pointLight.Pos.z);
        s_shader->setFloat3("u_Light.Ka", pointLight.Ka);
        s_shader->setFloat3("u_Light.Kd", pointLight.Kd);
        s_shader->setFloat3("u_Light.Ks", pointLight.Ks);
        s_shader->setFloat3("u_Light.Pos", pointLight.Pos);
        s_shader->setFloat3("u_Light.Intensity", pointLight.Intensity);
        s_shader->setMat4("u_Trans", pointLight.TransformComponent.getTransform());
    }

    void Renderer::updatePointLight(PointLight& pointLight) {
        s_shader->setFloat3("u_Light.Ka", {pointLight.Ka.x, pointLight.Ka.x, pointLight.Ka.x});
        s_shader->setFloat3("u_Light.Kd", {pointLight.Kd.x, pointLight.Kd.x, pointLight.Kd.x});
        s_shader->setFloat3("u_Light.Ks", {pointLight.Ks.x, pointLight.Ks.x, pointLight.Ks.x});
        s_shader->setFloat3("u_Light.Pos", {pointLight.Pos.x, pointLight.Pos.y, pointLight.Pos.z});
        s_shader->setFloat3("u_Light.Intensity", {pointLight.Intensity.x, pointLight.Intensity.x, pointLight.Intensity.x});
        s_shader->setMat4("u_Trans", pointLight.TransformComponent.getTransform());
    }
}
