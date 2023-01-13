#include "glad/glad.h"
#include "OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"
#include "Core/Timer.h"

#include "shaderc/shaderc.hpp"
#include <filesystem>



namespace RendererSpace {
    namespace Utils {
        static GLenum shaderTypeFromString(const std::string& type) {
            if (type == "vertex")
                return GL_VERTEX_SHADER;
            if (type == "fragment" || type == "pixel")
                return GL_FRAGMENT_SHADER;

            ASSERT(false, "Unknown shader type!");
            return 0;
        }

        static shaderc_shader_kind glShaderStageToShaderC(GLenum stage) {
            switch (stage) {
                case GL_VERTEX_SHADER:   return shaderc_glsl_vertex_shader;
                case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
            }
            ASSERT(false, "");
            return (shaderc_shader_kind)0;
        }

        static const char* glShaderStageToString(GLenum stage) {
            switch (stage) {
                case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
                case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
            }
            ASSERT(false, "");
            return nullptr;
        }

        static const char* getCacheDirectory() {
            return "../Assets/Cache/OpenGL";
        }

        static void createCacheDirectoryIfNeeded() {
            std::string cacheDirectory = getCacheDirectory();
            if (!std::filesystem::exists(cacheDirectory))
                std::filesystem::create_directories(cacheDirectory);
        }

        static const char* glShaderStageCachedOpenGLFileExtension(uint32_t stage) {
            switch (stage) {
                case GL_VERTEX_SHADER:    return ".cached_opengl.vert";
                case GL_FRAGMENT_SHADER:  return ".cached_opengl.frag";
            }
            ASSERT(false, "");
            return "";
        }

        static const char* glShaderStageCachedVulkanFileExtension(uint32_t stage) {
            switch (stage) {
                case GL_VERTEX_SHADER:    return ".cached_vulkan.vert";
                case GL_FRAGMENT_SHADER:  return ".cached_vulkan.frag";
            }
            ASSERT(false, "");
            return "";
        }
    }
}


RendererSpace::OpenGLShader::OpenGLShader(const std::string &filePath): m_filePath(filePath) {
    Utils::createCacheDirectoryIfNeeded();

    std::string source = readFile(filePath);
    auto shaderSources = preProcess(source);

    {
        Timer timer;
        compileOrGetVulkanBinaries(shaderSources);
        compileOrGetOpenGLBinaries();
        createProgram();
        LOG_INFO("Shader creation took {0} ms", timer.elapsedMillis());
    }
}

RendererSpace::OpenGLShader::OpenGLShader(std::string name, const std::string &vertexSrc, const std::string &fragmentSrc):
m_name(std::move(name)) {
    std::unordered_map<GLenum, std::string> sources;
    sources[GL_VERTEX_SHADER] = vertexSrc;
    sources[GL_FRAGMENT_SHADER] = fragmentSrc;

    compileOrGetVulkanBinaries(sources);
    compileOrGetOpenGLBinaries();
    createProgram();
}

RendererSpace::OpenGLShader::~OpenGLShader() {
    glDeleteProgram(m_rendererID);
}

void RendererSpace::OpenGLShader::bind() const {
    glUseProgram(m_rendererID);
}

void RendererSpace::OpenGLShader::unbind() const {
    glUseProgram(0);
}

GLint RendererSpace::OpenGLShader::getUniformLocation(const std::string &name) const {
    int location = glGetUniformLocation(m_rendererID, name.c_str());
    if(location == -1) {
        LOG_WARN("Uniform {0} is not used or exist", name);
    }
    return location;
}

void RendererSpace::OpenGLShader::uploadUniformInt(const std::string &name, int value) {
    GLint location = getUniformLocation(name);
    glUniform1i(location, value);
}

void RendererSpace::OpenGLShader::uploadUniformFloat(const std::string &name, float value) {
    GLint location = getUniformLocation(name);
    glUniform1f(location, value);
}

void RendererSpace::OpenGLShader::uploadUniformFloat2(const std::string &name, const glm::vec2 &value) {
    GLint location = getUniformLocation(name);
    glUniform2f(location, value.x, value.y);
}

void RendererSpace::OpenGLShader::uploadUniformFloat3(const std::string &name, const glm::vec3 &value) {
    GLint location = getUniformLocation(name);
    glUniform3f(location, value.x, value.y, value.z);
}

void RendererSpace::OpenGLShader::uploadUniformFloat4(const std::string &name, const glm::vec4 &value) {
    GLint location = getUniformLocation(name);
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void RendererSpace::OpenGLShader::uploadUniformMat3(const std::string &name, const glm::mat3 &mat) {
    GLint location = getUniformLocation(name);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void RendererSpace::OpenGLShader::uploadUniformMat4(const std::string &name, const glm::mat4 &mat) {
    GLint location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void RendererSpace::OpenGLShader::uploadUniformIntArray(const std::string &name, int *values, unsigned int count) {
    int location = getUniformLocation(name);
    glUniform1iv(location, count, values);
}

std::string RendererSpace::OpenGLShader::readFile(const std::string &filePath) {
    std::string result;
    std::ifstream in(filePath, std::ios::in | std::ios::binary);
    if(in) {
        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        if(size != -1) {
            result.resize(size);
            in.seekg(0, std::ios::beg);
            in.read(&result[0], size);
        }
        else {
            LOG_ERROR("Could not read from file '{0}'", filePath);
        }
    }
    else {
        LOG_ERROR("Could not open file '{0}'", filePath);
    }
    return result;
}

std::unordered_map<GLenum, std::string> RendererSpace::OpenGLShader::preProcess(const std::string &source) {
    std::unordered_map<GLenum, std::string> shaderSources;
    const char* typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = source.find(typeToken, 0);
    while (pos != std::string::npos)
    {
        size_t eol = source.find_first_of("\r\n", pos);
        ASSERT(eol != std::string::npos, "Syntax error");
        size_t begin = pos + typeTokenLength + 1;
        std::string type = source.substr(begin, eol - begin);
        ASSERT(Utils::shaderTypeFromString(type), "Invalid shader type specified");

        size_t nextLinePos = source.find_first_not_of("\r\n", eol);
        ASSERT(nextLinePos != std::string::npos, "Syntax error");
        pos = source.find(typeToken, nextLinePos);

        shaderSources[Utils::shaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos)
                : source.substr(nextLinePos, pos - nextLinePos);
    }

    return shaderSources;
}


void RendererSpace::OpenGLShader::compileOrGetOpenGLBinaries() {
    auto& shaderData = m_openGLSPIRV;

    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
    const bool optimize = false;
    if(optimize)
        options.SetOptimizationLevel(shaderc_optimization_level_performance);

    std::filesystem::path cacheDirectory = Utils::getCacheDirectory();

    shaderData.clear();
    m_openGLSourceCode.clear();

    for(auto&& [stage, spirv] : m_vulkanSPIRV) {
        std::filesystem::path shaderFilePath = m_filePath;
        std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() +
                Utils::glShaderStageCachedOpenGLFileExtension(stage));

        std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
        if(in.is_open()) {
            in.seekg(0, std::ios::end);
            auto size = in.tellg();
            in.seekg(0, std::ios::beg);

            auto& data = shaderData[stage];
            data.resize(size / sizeof(uint32_t));
            in.read((char*)data.data(), size);
        }
        else {
            spirv_cross::CompilerGLSL glslCompiler(spirv);
            m_openGLSourceCode[stage] = glslCompiler.compile();
            auto& source = m_openGLSourceCode[stage];

            shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source,
                                                                             Utils::glShaderStageToShaderC(stage),
                                                                             m_filePath.c_str());
            if(module.GetCompilationStatus() != shaderc_compilation_status_success) {
                LOG_ERROR(module.GetErrorMessage());

                ASSERT(false, "");
            }
            shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

            std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
            if(out.is_open()) {
                auto& data = shaderData[stage];
                out.write((char*)data.data(), data.size() * sizeof(uint32_t));
                out.flush();
                out.close();
            }
        }
    }
}

void RendererSpace::OpenGLShader::createProgram() {
    GLuint program = glCreateProgram();

    std::vector<GLuint> shaderIDs;
    for(auto&& [stage, spirv] : m_openGLSPIRV) {
        GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
        glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
        glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
        glAttachShader(program, shaderID);
    }

    glLinkProgram(program);

    GLint isLinked;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if(isLinked == GL_FALSE) {
        GLint maxLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
        LOG_ERROR("Shader linking failed ({0}):\n{1}", m_filePath, infoLog.empty()? " " : infoLog.data());
        glDeleteProgram(program);
        for(auto id : shaderIDs)
            glDeleteShader(id);
    }
    for(auto id : shaderIDs) {
        glDetachShader(program, id);
        glDeleteShader(id);
    }
    m_rendererID = program;
}

void RendererSpace::OpenGLShader::reflect(GLenum stage, const std::vector<uint32_t> &shaderData) {
    spirv_cross::Compiler compiler(shaderData);
    spirv_cross::ShaderResources resources = compiler.get_shader_resources();

    LOG_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::glShaderStageToString(stage), m_filePath);
    LOG_TRACE("{0} uniform buffers", resources.uniform_buffers.size());
    LOG_TRACE("{0} resources", resources.sampled_images.size());

    LOG_TRACE("Uniform buffers:");
    for(const auto& resource : resources.uniform_buffers) {
        const auto& bufferType = compiler.get_type(resource.base_type_id);
        uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
        uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
        int memberCount = bufferType.member_types.size();

        LOG_TRACE("{0}", resource.name);
        LOG_TRACE("Size = {0}", bufferSize);
        LOG_TRACE("Binding = {0}", binding);
        LOG_TRACE("Members = {0}", memberCount);
    }
}

void RendererSpace::OpenGLShader::compileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string> &shaderSources) {
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
    const bool optimize = true;
    if(optimize)
        options.SetOptimizationLevel(shaderc_optimization_level_performance);

    std::filesystem::path cacheDirectory = Utils::getCacheDirectory();
    auto& shaderData = m_vulkanSPIRV;
    shaderData.clear();
    for(auto&& [stage, source] : shaderSources) {
        std::filesystem::path shaderFilePath = m_filePath;
        std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() +
                                                             Utils::glShaderStageCachedVulkanFileExtension(stage));

        std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
        if (in.is_open()) {
            in.seekg(0, std::ios::end);
            auto size = in.tellg();
            in.seekg(0, std::ios::beg);
            auto& data = shaderData[stage];
            data.resize(size / sizeof(uint32_t));
            in.read((char*)data.data(), size);
        }
        else {
            shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source,
                                                                             Utils::glShaderStageToShaderC(stage),
                                                                             m_filePath.c_str(), options);
            if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
                LOG_ERROR(module.GetErrorMessage());
                ASSERT(false, "");
            }

            shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

            std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
            if(out.is_open()) {
                auto& data = shaderData[stage];
                out.write((char*)data.data(), data.size() * sizeof(uint32_t));
                out.flush();
                out.close();
            }
        }
    }
    for (auto&& [stage, data]: shaderData)
        reflect(stage, data);
}
