#pragma once
#include <vector>
#include "Mesh.h"
#include "Material.h"
#include "GraphicsAPI.h"

namespace why
{

    struct RenderCommand
    {
        Mesh* mesh = nullptr;
        Material* material = nullptr;
        glm::mat4 modelMatrix;
    };

    struct CameraData
    {
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
    };

    class RenderQueue
    {
    public:
        void Submit(const RenderCommand& command);
        void Draw(const CameraData& cameraData);

    private:
        std::vector<RenderCommand> m_commands;
    };
}