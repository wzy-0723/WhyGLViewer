#include "TestObject.h"
#include <iostream>
#include "GraphicsAPI.h"
#include "Engine.h"
#include <qevent.h>
#include "MeshComponent.h"
TestObject::TestObject()
{
    auto shaderProgram = SINGLETON_PTR(why::GraphicsAPI)->CreateShaderProgram("1.model_loading");
    auto material = std::make_shared<why::Material>();
    material->SetShaderProgram(shaderProgram);

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

    auto mesh = std::make_shared<why::Mesh>(vertexLayout, vertices, indices);
    AddComponent(new why::MeshComponent(material, mesh));
}

void TestObject::Update(float deltaTime)
{
    why::GameObject::Update(deltaTime);

    //auto input = SINGLETON_PTR(why::Engine)->GetInputManager();

    //auto position = GetPosition();
    //// Horizontal movement
    //if (input->IsKeyPressed(Qt::Key_A))
    //{
    //    position.x -= 0.001f;
    //}
    //else if (input->IsKeyPressed(Qt::Key_D))
    //{
    //    position.x += 0.001f;
    //}
    //// Vertical movement
    //if (input->IsKeyPressed(Qt::Key_W))
    //{
    //    position.y += 0.001f;
    //}
    //else if (input->IsKeyPressed(Qt::Key_S))
    //{
    //    position.y -= 0.001f;
    //}
    //SetPosition(position);
}