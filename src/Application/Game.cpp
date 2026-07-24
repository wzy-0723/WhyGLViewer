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
#include "AnimationComponent.h"
#include "PhysicsComponent.h"
#include "Player.h"
bool Game::Init()
{
    SINGLETON_PTR(why::Engine)->SetScene(&m_scene);

    auto player = m_scene.CreateObject<Player>("Player");
    player->Init();
    m_scene.SetMainCamera(player);

    m_scene.CreateObject<TestObject>("TestObject");

    auto material = why::Material::Load("materials/brick.mat");

    auto mesh = why::Mesh::CreateBox();

    auto objectB = m_scene.CreateObject("ObjectB");
    objectB->AddComponent(new why::MeshComponent(material, mesh));
    objectB->SetPosition(glm::vec3(0.0f, 2.0f, 2.0f));
    objectB->SetRotation(glm::vec3(0.0f, 2.0f, 0.0f));

    auto objectC = m_scene.CreateObject("ObjectC");
    objectC->AddComponent(new why::MeshComponent(material, mesh));
    objectC->SetPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
    objectC->SetRotation(glm::vec3(1.0f, 0.0f, 1.0f));
    objectC->SetScale(glm::vec3(1.5f, 1.5f, 1.5f));

    auto suzanneObject = why::GameObject::LoadGLTF("Models/suzanne/Suzanne.gltf");
    suzanneObject->SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));

    auto light = m_scene.CreateObject("Light");
    auto lightComp = new why::LightComponent();
    lightComp->SetColor(glm::vec3(1.0f));
    light->AddComponent(lightComp);
    light->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));

    auto ground = m_scene.CreateObject("Ground");
    ground->SetPosition(glm::vec3(0.0f, -5.0f, 0.0f));

    glm::vec3 groundExtents(20.0f, 2.0f, 20.0f);
    auto groundMesh = why::Mesh::CreateBox(groundExtents);
    ground->AddComponent(new why::MeshComponent(material, groundMesh));

    auto groundCollider = std::make_shared<why::BoxCollider>(groundExtents);
    auto groundBody = std::make_shared<why::RigidBody>(
        why::BodyType::Static, groundCollider, 0.0f, 0.5f);
    ground->AddComponent(new why::PhysicsComponent(groundBody));

    auto boxObj = m_scene.CreateObject("FallingBox");
    boxObj->AddComponent(new why::MeshComponent(material, mesh));
    boxObj->SetPosition(glm::vec3(0.0f, 2.0f, 2.0f));
    boxObj->SetRotation(glm::quat(glm::vec3(1.0f, 2.0f, 0.0f)));
    auto boxCollider = std::make_shared<why::BoxCollider>(glm::vec3(1.0f));
    auto boxBody = std::make_shared<why::RigidBody>(
        why::BodyType::Dynamic, boxCollider, 5.0f, 0.5f);
    boxObj->AddComponent(new why::PhysicsComponent(boxBody));
    
    return true;
}

void Game::Update(float deltaTime)
{
    m_scene.Update(deltaTime);
}

void Game::Destroy()
{

}


