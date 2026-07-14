#pragma once
#include <QOpenGLShaderProgram>
#include <unordered_map>
#include <iostream>

namespace why
{
    class Material
    {
    public:
        void SetShaderProgram(const std::shared_ptr<QOpenGLShaderProgram>& shaderProgram);
        void SetParam(const std::string& name, float value);
        void Bind();

    private:
        std::shared_ptr<QOpenGLShaderProgram> m_shaderProgram;
        std::unordered_map<std::string, float> m_floatParams;
    };
}
