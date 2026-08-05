#pragma once
#include <vector>
#include <memory>
#include "Mesh.h"
#include "Material.h"
#include "GraphicsAPI.h"
#include "Common.h"
#include "Texture.h"
namespace why
{

    struct RenderCommand
    {
        Mesh* mesh = nullptr;
        Material* material = nullptr;
        glm::mat4 modelMatrix;
    };

    struct RenderCommand2D
    {
        glm::mat4 modelMatrix;
        Texture* texture = nullptr;
        glm::vec4 color;
        glm::vec2 size;
        glm::vec2 lowerLeftUV;
        glm::vec2 upperRightUV;
        glm::vec2 pivot;
    };

    class RenderQueue
    {
    public:
        void Init();
        void Submit(const RenderCommand& command);
        void Submit(const RenderCommand2D& command);
        void Draw(const CameraData& cameraData, const std::vector<LightData>& lights);

    private:
        std::vector<RenderCommand> m_commands;
        std::vector<RenderCommand2D> m_commands2D;
        std::shared_ptr<Mesh> m_mesh2D;
        std::shared_ptr<Mesh> m_mesh;
    };
}