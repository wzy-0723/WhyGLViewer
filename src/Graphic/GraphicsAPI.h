#pragma once
#include <memory>
#include <string>
#include <QOpenGLShaderProgram>
#include "Singleton.h"
#include "Material.h"
#include "Mesh.h"
#include "ShaderProgram.h"
namespace why
{
    class GraphicsAPI
    {
    public:
        template<typename> friend class Singleton;
        
        std::shared_ptr<QOpenGLShaderProgram> CreateShaderProgram(const std::string& strShaderName);

        GLuint CreateVertexBuffer(const std::vector<float>& vertices);
        GLuint CreateIndexBuffer(const std::vector<uint32_t>& indices);

        void SetClearColor(float r, float g, float b, float a);
        void ClearBuffers();
        bool Init();
        
        void BindShaderProgram(ShaderProgram* shaderProgram);
        void BindMaterial(Material* material);
        void BindMesh(Mesh* mesh);
        void DrawMesh(Mesh* mesh);

    private:
        GraphicsAPI() {};
    };
}