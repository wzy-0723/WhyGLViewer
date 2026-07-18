#pragma once
#include <vector>
#include "Mesh.h"
#include "Material.h"
#include "GraphicsAPI.h"
#include "Common.h"
namespace why
{

    struct RenderCommand
    {
        Mesh* mesh = nullptr;
        Material* material = nullptr;
        glm::mat4 modelMatrix;
    };

    class RenderQueue
    {
    public:
        void Submit(const RenderCommand& command);
        void Draw(const CameraData& cameraData, const std::vector<LightData>& lights);

    private:
        std::vector<RenderCommand> m_commands;
    };
}