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
    enum class BlendMode
    {
        Disabled,
        Alpha,
        Additive,
        Multiply
    };

    struct Rect
    {
        int x = 0;
        int y = 0;
        int width = 0;
        int height = 0;
    };


    class GraphicsAPI
    {
    public:
        template<typename> friend class Singleton;
        
        std::shared_ptr<ShaderProgram> CreateShaderProgram(const std::string& vertexSource,
            const std::string& fragmentSource);

        const std::shared_ptr<ShaderProgram>& GetDefault2DShaderProgram();
        const std::shared_ptr<ShaderProgram>& GetDefaultShaderProgram();
        const std::shared_ptr<ShaderProgram>& GetDefaultUIShaderProgram();

        //std::shared_ptr<QOpenGLShaderProgram> CreateShaderProgram(const std::string& strShaderName);

        GLuint CreateVertexBuffer(const std::vector<float>& vertices);
        GLuint CreateIndexBuffer(const std::vector<uint32_t>& indices);

        void SetClearColor(float r, float g, float b, float a);
        void ClearBuffers();
        const Rect& GetViewport() const;
        void SetViewport(int x, int y, int width, int height);
        bool Init();
        
        void BindShaderProgram(ShaderProgram* shaderProgram);
        void BindMaterial(Material* material);
        void BindMesh(Mesh* mesh);
        void UnbindMesh(Mesh* mesh);
        void DrawMesh(Mesh* mesh);

        void SetDepthTestEnabled(bool enabled);
        void SetBlendMode(BlendMode mode);

    private:
        GraphicsAPI() {};
    private:
        Rect m_viewport;
        std::shared_ptr<ShaderProgram> m_defaultShaderProgram;
        std::shared_ptr<ShaderProgram> m_default2DShaderProgram;
        std::shared_ptr<ShaderProgram> m_defaultUIShaderProgram;
    };
}