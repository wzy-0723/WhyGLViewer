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
    };

    class RenderQueue
    {
    public:
        void Submit(const RenderCommand& command);
        void Draw();

    private:
        std::vector<RenderCommand> m_commands;
    };
}