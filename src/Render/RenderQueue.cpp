#include "RenderQueue.h"


namespace why
{
    void RenderQueue::Submit(const RenderCommand& command)
    {
        m_commands.push_back(command);
    }

    void RenderQueue::Draw(const CameraData& cameraData, const std::vector<LightData>& lights)
    {
        // 3D
        for (auto& command : m_commands)
        {          
            SINGLETON_PTR(GraphicsAPI)->BindMaterial(command.material);
            
            auto shaderProgram = command.material->GetShaderProgram();

            shaderProgram->SetUniform("uModel", command.modelMatrix);
            shaderProgram->SetUniform("uView", cameraData.viewMatrix);
            shaderProgram->SetUniform("uProjection", cameraData.projectionMatrix);
            shaderProgram->SetUniform("uCameraPos", cameraData.position);

            if (!lights.empty())
            {
                auto& light = lights[0];
                shaderProgram->SetUniform("uLight.color", light.color);
                shaderProgram->SetUniform("uLight.direction", glm::normalize(-light.position));
                //shaderProgram->SetUniform("uLight.position", light.position);
            }

            SINGLETON_PTR(GraphicsAPI)->BindMesh(command.mesh);
            SINGLETON_PTR(GraphicsAPI)->DrawMesh(command.mesh);
            SINGLETON_PTR(GraphicsAPI)->UnbindMesh(command.mesh);
        }

        m_commands.clear();

        // 2D
        SINGLETON_PTR(GraphicsAPI)->SetDepthTestEnabled(false);
        SINGLETON_PTR(GraphicsAPI)->SetBlendMode(BlendMode::Alpha);
        const auto shaderProgram2D = SINGLETON_PTR(GraphicsAPI)->GetDefault2DShaderProgram();
        shaderProgram2D->Bind();
        m_mesh2D->Bind();
        for (auto& command : m_commands2D)
        {
            // rendering
            shaderProgram2D->SetUniform("uModel", command.modelMatrix);
            shaderProgram2D->SetUniform("uView", cameraData.viewMatrix);
            shaderProgram2D->SetUniform("uProjection", cameraData.orthoMatrix);
            shaderProgram2D->SetUniform("uSize", command.size.x, command.size.y);
            shaderProgram2D->SetUniform("uPivot", command.pivot.x, command.pivot.y);
            shaderProgram2D->SetUniform("uUVMin", command.lowerLeftUV.x, command.lowerLeftUV.y);
            shaderProgram2D->SetUniform("uUVMax", command.upperRightUV.x, command.upperRightUV.y);
            shaderProgram2D->SetUniform("uColor", command.color);
            shaderProgram2D->SetTexture("uTex", command.texture);
            m_mesh2D->Draw();

        }
        m_mesh2D->Unbind();
        SINGLETON_PTR(GraphicsAPI)->SetBlendMode(BlendMode::Disabled);
        SINGLETON_PTR(GraphicsAPI)->SetDepthTestEnabled(true);
        m_commands2D.clear();
    }

    void RenderQueue::Init()
    {
        m_mesh2D = Mesh::CreatePlane();
    }

    void RenderQueue::Submit(const RenderCommand2D& command)
    {
        m_commands2D.push_back(command);
    }
}