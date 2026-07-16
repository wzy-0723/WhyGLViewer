#include "TestObject.h"
#include <iostream>
#include "GraphicsAPI.h"
#include "Engine.h"
#include <qevent.h>
TestObject::TestObject()
{
    auto shaderProgram = SINGLETON_PTR(why::GraphicsAPI)->CreateShaderProgram("1.model_loading");
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

    m_mesh = std::make_shared<why::Mesh>(vertexLayout, vertices, indices);
}

void TestObject::Update(float deltaTime)
{
    why::GameObject::Update(deltaTime);

    auto input = SINGLETON_PTR(why::Engine)->GetInputManager();

    // Horizontal movement
    if (input->IsKeyPressed(Qt::Key_A))
    {
        m_offsetX -= 0.001f;
    }
    else if (input->IsKeyPressed(Qt::Key_D))
    {
        m_offsetX += 0.001f;
    }
    // Vertical movement
    if (input->IsKeyPressed(Qt::Key_W))
    {
        m_offsetY += 0.001f;
    }
    else if (input->IsKeyPressed(Qt::Key_S))
    {
        m_offsetY -= 0.001f;
    }

    m_material.SetParam("uOffset", m_offsetX, m_offsetY);

    why::RenderCommand command;
    command.material = &m_material;
    command.mesh = m_mesh.get();

    SINGLETON_PTR(why::Engine)->GetRenderQueue()->Submit(command);
}