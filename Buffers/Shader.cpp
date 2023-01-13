#include "Shader.h"
#include "Core/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

RendererSpace::Ref<RendererSpace::Shader> RendererSpace::Shader::createShader(const std::string &filePath) {
    switch (Renderer::getRendererAPI()) {
        case RendererAPI::None:
            ASSERT(false, "RendererAPI::API::None is currently not supported");
            return nullptr;
        case RendererAPI::OpenGL:
            return createRef<OpenGLShader>(filePath);
    }
    return nullptr;
}

RendererSpace::Ref<RendererSpace::Shader> RendererSpace::Shader::createShader(const std::string& name, const std::string &vertexSrc, const std::string &fragmentSrc) {
    switch (Renderer::getRendererAPI()) {
        case RendererAPI::None:
            ASSERT(false, "RendererAPI::None is currently not supported");
            return nullptr;
        case RendererAPI::OpenGL:
            return createRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
    }
    return nullptr;
}



void RendererSpace::ShaderLibrary::addShader(RendererSpace::Ref<RendererSpace::Shader> &shader) {
    ASSERT(!exists(shader->getName()), "Shader already exists");
    m_shaders[shader->getName()] = shader;
}

void RendererSpace::ShaderLibrary::addShader(const std::string &name, RendererSpace::Ref<RendererSpace::Shader> &shader) {
    ASSERT(!exists(name), "Shader already exists");
    m_shaders[name] = shader;
}

void RendererSpace::ShaderLibrary::addShader(const std::string &name, const std::string &filePath) {
    ASSERT(!exists(name), "Shader already exists");
    auto shader = RendererSpace::Shader::createShader(filePath);
    addShader(name, shader);
}

RendererSpace::Ref<RendererSpace::Shader> RendererSpace::ShaderLibrary::getShader(const std::string &name) {
    return RendererSpace::Ref<RendererSpace::Shader>();
}

bool RendererSpace::ShaderLibrary::exists(const std::string &name) const {
    return false;
}

RendererSpace::Ref<RendererSpace::Shader> RendererSpace::ShaderLibrary::loadShader(const std::string &filePath) {
    auto shader = Shader::createShader(filePath);
    addShader(shader->getName(), shader);
    return shader;
}

RendererSpace::Ref<RendererSpace::Shader> RendererSpace::ShaderLibrary::loadShader(const std::string &name, const std::string &vertexSrc,
                                                           const std::string &fragmentSrc) {
    auto shader = Shader::createShader(name, vertexSrc, fragmentSrc);
    addShader(name, shader);
    return shader;
}

