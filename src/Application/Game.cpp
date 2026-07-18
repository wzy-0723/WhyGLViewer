#include "Game.h"
#include <iostream>
#include "RenderQueue.h"
#include "TestObject.h"
#include "Engine.h"
#include "CameraComponent.h"
#include "PlayerControllerComponent.h"
#include "MeshComponent.h"
#include "Logger.h"
#include "LightComponent.h"

bool Game::Init()
{
    auto camera = m_scene.CreateObject("Camera");
    camera->AddComponent(new why::CameraComponent());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    camera->AddComponent(new why::PlayerControllerComponent());
    m_scene.SetMainCamera(camera);

    m_scene.CreateObject<TestObject>("TestObject");

    auto material = why::Material::Load("materials/brick.mat");

    auto mesh = why::Mesh::CreateCube();

    auto objectA = m_scene.CreateObject("ObjectA");
    objectA->AddComponent(new why::MeshComponent(material, mesh));
    objectA->SetPosition(glm::vec3(1.0f, 0.0f, -5.0f));

    auto objectB = m_scene.CreateObject("ObjectB");
    objectB->AddComponent(new why::MeshComponent(material, mesh));
    objectB->SetPosition(glm::vec3(0.0f, 2.0f, 2.0f));
    objectB->SetRotation(glm::vec3(0.0f, 2.0f, 0.0f));

    auto objectC = m_scene.CreateObject("ObjectC");
    objectC->AddComponent(new why::MeshComponent(material, mesh));
    objectC->SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
    objectC->SetRotation(glm::vec3(1.0f, 0.0f, 1.0f));
    objectC->SetScale(glm::vec3(1.5f, 1.5f, 1.5f));

    auto suzanneMesh = why::Mesh::Load("models/Suzanne.gltf");
    auto suzanneMaterial = why::Material::Load("materials/suzanne.mat");

    auto suzanneObj = m_scene.CreateObject("Suzanne");
    suzanneObj->AddComponent(new why::MeshComponent(suzanneMaterial, suzanneMesh));
    suzanneObj->SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));

    auto light = m_scene.CreateObject("Light");
    auto lightComp = new why::LightComponent();
    lightComp->SetColor(glm::vec3(1.0f));
    light->AddComponent(lightComp);
    light->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));

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


