#include "RenderQueue.h"


namespace why
{
    void RenderQueue::Submit(const RenderCommand& command)
    {
        m_commands.push_back(command);
    }

    void RenderQueue::Draw()
    {
        for (auto& command : m_commands)
        {          
            SINGLETON_PTR(GraphicsAPI)->BindMaterial(command.material);
            SINGLETON_PTR(GraphicsAPI)->BindMesh(command.mesh);
            SINGLETON_PTR(GraphicsAPI)->DrawMesh(command.mesh);
        }

        m_commands.clear();
    }
}