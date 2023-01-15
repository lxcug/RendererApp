#include "glad/glad.h"
#include "OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"
#include "Core/Timer.h"

#include "shaderc/shaderc.hpp"
#include <filesystem>



namespace RendererSpace {
    static GLenum ShaderTypeFromString(const std::string& type) {
        if (type == "vertex")
            return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel")
            return GL_FRAGMENT_SHADER;

        ASSERT(false, "Unknown shader type!");
        return 0;
    }
}


RendererSpace::OpenGLShader::OpenGLShader(const std::string &filePath): m_filePath(filePath) {
    {
        Timer timer;
        std::string source = readFile(filePath);
        auto shaderSources = preProcess(source);
        compile(shaderSources);
        LOG_INFO("Compile Shader takes {}s", timer.elapsedS());
    }
}

RendererSpace::OpenGLShader::OpenGLShader(std::string name, const std::string &vertexSrc, const std::string &fragmentSrc):
m_name(std::move(name)) {
    {
        Timer timer;
        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;
        compile(sources);
        LOG_INFO("Compile Shader takes {}s", timer.elapsedS());
    }
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
    size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
    while(pos != std::string::npos) {
        size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
        ASSERT(eol != std::string::npos, "Syntax error");
        size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
        std::string type = source.substr(begin, eol - begin);
        ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

        size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
        ASSERT(nextLinePos != std::string::npos, "Syntax error");
        pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

        shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
    }

    return shaderSources;
}

void RendererSpace::OpenGLShader::compile(const std::unordered_map<GLenum, std::string> &shaderSources) {
    GLuint program = glCreateProgram();
    ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");

    std::array<GLenum, 2> glShaderIDs;
    int glShaderIDIndex = 0;
    for(auto& kv : shaderSources) {
        GLenum type = kv.first;
        const std::string& source = kv.second;

        GLuint shader = glCreateShader(type);

        const GLchar* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, nullptr);

        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            glDeleteShader(shader);

            LOG_ERROR("{0}", infoLog.empty()? "Info log is empty!" : infoLog.data());
            ASSERT(false, "Shader compilation failure!");
            break;
        }

        glAttachShader(program, shader);
        glShaderIDs[glShaderIDIndex++] = shader;
    }

    m_rendererID = program;

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if(isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);

        for (auto id : glShaderIDs)
            glDeleteShader(id);

        LOG_ERROR("{0}", infoLog.empty()? "Info log is empty!" : infoLog.data());
        ASSERT(false, "Shader lnk failure!");
        return;
    }

    for (auto id : glShaderIDs)
    {
        glDetachShader(program, id);
        glDeleteShader(id);
    }
}
