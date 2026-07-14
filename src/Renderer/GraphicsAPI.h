#pragma once
#include <memory>
#include <string>
#include <QOpenGLShaderProgram>


namespace why
{
    class Material;

    class GraphicsAPI
    {
    public:
        std::shared_ptr<QOpenGLShaderProgram> CreateShaderProgram(const std::string& strShaderName);

        void BindMaterial(Material* material);
    };
}