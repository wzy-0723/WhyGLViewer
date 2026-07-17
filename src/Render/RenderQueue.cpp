#include "RenderQueue.h"


namespace why
{
    void RenderQueue::Submit(const RenderCommand& command)
    {
        m_commands.push_back(command);
    }

    void RenderQueue::Draw(const CameraData& cameraData)
    {
        for (auto& command : m_commands)
        {          
            SINGLETON_PTR(GraphicsAPI)->BindMaterial(command.material);
            
            auto shaderProgram = command.material->GetShaderProgram();

            shaderProgram->SetUniform("uModel", command.modelMatrix);
            shaderProgram->SetUniform("uView", cameraData.viewMatrix);
            shaderProgram->SetUniform("uProjection", cameraData.projectionMatrix);

            SINGLETON_PTR(GraphicsAPI)->BindMesh(command.mesh);
            SINGLETON_PTR(GraphicsAPI)->DrawMesh(command.mesh);
        }

        m_commands.clear();
    }
}