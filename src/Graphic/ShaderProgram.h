#pragma once
#include <string>
#include <unordered_map>
#include <glm/mat4x4.hpp>
#include <QOpenGLShaderProgram>
#include "Texture.h"
namespace why
{
    class ShaderProgram
    {
    public:
        ShaderProgram() = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        explicit ShaderProgram(GLuint shaderProgramID);
        explicit ShaderProgram(const std::shared_ptr<QOpenGLShaderProgram>& pQShader);

        ~ShaderProgram();

        void Bind();
        GLint GetUniformLocation(const std::string& name);
        void SetUniform(const std::string& name, float value);
        void SetUniform(const std::string& name, float v0, float v1);
        void SetUniform(const std::string& name, const glm::mat4& mat);
        void SetTexture(const std::string& name, Texture* texture);

    private:
        int m_currentTextureUnit = 0;
        std::shared_ptr<QOpenGLShaderProgram> m_pQShader;
        std::unordered_map<std::string, GLint> m_uniformLocationCache;
        GLuint m_shaderProgramID = 0;
    };
}