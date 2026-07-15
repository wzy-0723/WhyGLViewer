#include "Game.h"
#include <iostream>
#include "RenderQueue.h"

namespace why
{
    bool Game::Init()
    {     
        auto shaderProgram = SINGLETON_PTR(GraphicsAPI)->CreateShaderProgram("1.model_loading");
        m_material.SetShaderProgram(shaderProgram);

        std::vector<float> vertices =
        {
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f
        };

        std::vector<unsigned int> indices =
        {
            0, 1, 2,
            0, 2, 3
        };

        why::VertexLayout vertexLayout;

        // Postion
        vertexLayout.elements.push_back({
            0,
            3,
            GL_FLOAT,
            0
            });
        // Color
        vertexLayout.elements.push_back({
            1,
            3,
            GL_FLOAT,
            sizeof(float) * 3
            });
        vertexLayout.stride = sizeof(float) * 6;

        m_mesh = std::make_unique<why::Mesh>(vertexLayout, vertices, indices);

        return true;
    }

    void Game::Update(const QEvent& e)
    {
        why::RenderCommand command;
        command.material = &m_material;
        command.mesh = m_mesh.get();

        auto& renderQueue = Application::GetRenderQueue();
        renderQueue.Submit(command);
    }

    void Game::Destroy()
    {

    }
}

