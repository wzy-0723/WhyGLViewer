#pragma once
#include <QOpenGLShaderProgram>
#include <unordered_map>
#include <iostream>
#include "ShaderProgram.h"
namespace why
{
    class Material
    {
    public:
        void SetShaderProgram(const std::shared_ptr<QOpenGLShaderProgram>& shaderProgram);
        void SetParam(const std::string& name, float value);
        void SetParam(const std::string& name, float v0, float v1);

        void Bind();

        ShaderProgram* GetShaderProgram() { return m_pShaderProgram.get(); };
    private:
        std::shared_ptr<ShaderProgram> m_pShaderProgram;
        std::unordered_map<std::string, float> m_floatParams;
        std::unordered_map<std::string, std::pair<float, float>> m_float2Params;
    };
}
