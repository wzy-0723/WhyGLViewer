#include "Game.h"
#include <iostream>
#include "RenderQueue.h"
#include "TestObject.h"
#include "Engine.h"
#include "CameraComponent.h"
#include "PlayerControllerComponent.h"
#include "MeshComponent.h"
bool Game::Init()
{     

    auto camera = m_scene.CreateObject("Camera");
    camera->AddComponent(new why::CameraComponent());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    camera->AddComponent(new why::PlayerControllerComponent());
    m_scene.SetMainCamera(camera);

    m_scene.CreateObject<TestObject>("TestObject");

    auto shaderProgram = SINGLETON_PTR(why::GraphicsAPI)->CreateShaderProgram("1.model_loading");
    auto material = std::make_shared<why::Material>();
    material->SetShaderProgram(shaderProgram);

    std::vector<float> vertices =
    {
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f,

        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f
    };

    std::vector<unsigned int> indices =
    {
        // front face
        0, 1, 2,
        0, 2, 3,
        // top face
        4, 5, 1,
        4, 1, 0,
        // right face
        4, 0, 3,
        4, 3, 7,
        // left face
        1, 5, 6,
        1, 6, 2,
        // bottom face
        3, 2, 6,
        3, 6, 7,
        // back face
        4, 7, 6,
        4, 6, 5
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

    auto objectA = m_scene.CreateObject("ObjectA");
    objectA->AddComponent(new why::MeshComponent(material, mesh));
    objectA->SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));

    auto objectB = m_scene.CreateObject("ObjectB");
    objectB->AddComponent(new why::MeshComponent(material, mesh));
    objectB->SetPosition(glm::vec3(0.0f, 2.0f, 2.0f));
    objectB->SetRotation(glm::vec3(0.0f, 2.0f, 0.0f));

    auto objectC = m_scene.CreateObject("ObjectC");
    objectC->AddComponent(new why::MeshComponent(material, mesh));
    objectC->SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
    objectC->SetRotation(glm::vec3(1.0f, 0.0f, 1.0f));
    objectC->SetScale(glm::vec3(1.5f, 1.5f, 1.5f));

    SINGLETON_PTR(why::Engine)->SetScene(&m_scene);
    return true;
}

void Game::Update(float deltaTime)
{
    m_scene.Update(deltaTime);
}

void Game::Destroy()
{

}


