#pragma once
#include <memory>
#include <string>
#include <QOpenGLShaderProgram>
#include "Singleton.h"

namespace why
{
    class Material;

    class GraphicsAPI
    {
    public:
        template<typename> friend class Singleton;
        std::shared_ptr<QOpenGLShaderProgram> CreateShaderProgram(const std::string& strShaderName);

        void BindMaterial(Material* material);

        GLuint CreateVertexBuffer(const std::vector<float>& vertices);
        GLuint CreateIndexBuffer(const std::vector<uint32_t>& indices);
    private:
        GraphicsAPI() {};
    };
}