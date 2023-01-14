// @author: lx
// @time: 2022/10/17
// @brief: 

#pragma once
#ifndef GAME_OPENGLSHADER_H
#define GAME_OPENGLSHADER_H

#include "Core/Core.h"
#include "Buffers//Shader.h"

#include "GLFW/glfw3.h"
#include <fstream>
#include "spirv_cross.hpp"
#include "spirv_glsl.hpp"



namespace RendererSpace {
    class OpenGLShader : public Shader {
    public:
        OpenGLShader(const std::string& filePath);
        OpenGLShader(std::string name, const std::string& vertexSrc, const std::string& fragmentSrc);
        ~OpenGLShader();

        void bind() const override;
        void unbind() const override;

        const std::string& getName() const override {
            return m_name;
        }

        [[nodiscard]] inline GLint getUniformLocation(const std::string& name) const;

        void setInt(const std::string &name, int value) override {
            uploadUniformInt(name, value);
        }
        void setFloat(const std::string &name, float value) override {
            uploadUniformFloat(name, value);
        }
        void setFloat3(const std::string &name, const glm::vec3 &value) override {
            uploadUniformFloat3(name, value);
        }
        void setFloat4(const std::string &name, const glm::vec4 &value) override {
            uploadUniformFloat4(name, value);
        }
        void setMat4(const std::string &name, const glm::mat4 &value) override {
            uploadUniformMat4(name, value);
        }
        void setIntArray(const std::string &name, int *values, unsigned int count) override {
            uploadUniformIntArray(name, values, count);
        }

        void uploadUniformInt(const std::string& name, int value);
        void uploadUniformFloat(const std::string& name, float value);
        void uploadUniformFloat2(const std::string& name, const glm::vec2& value);
        void uploadUniformFloat3(const std::string& name, const glm::vec3& value);
        void uploadUniformFloat4(const std::string& name, const glm::vec4& value);

        void uploadUniformMat3(const std::string& name, const glm::mat3& mat);
        void uploadUniformMat4(const std::string& name, const glm::mat4& mat);

        void uploadUniformIntArray(const std::string& name, int* values, unsigned count);

    private:
        std::string readFile(const std::string& filePath);
        std::unordered_map<GLenum, std::string> preProcess(const std::string& source);
        void compile(const std::unordered_map<GLenum, std::string>& shaderSources);

    private:
        unsigned m_rendererID;
        std::string m_name;
        std::string m_filePath;
    };
}


#endif //GAME_OPENGLSHADER_H
