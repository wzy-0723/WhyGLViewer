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

bool Game::Init()
{
    SINGLETON_PTR(why::Engine)->SetScene(&m_scene);

    auto camera = m_scene.CreateObject("Camera");
    camera->AddComponent(new why::CameraComponent());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    camera->AddComponent(new why::PlayerControllerComponent());
    m_scene.SetMainCamera(camera);

    m_scene.CreateObject<TestObject>("TestObject");

    auto material = why::Material::Load("materials/brick.mat");

    auto mesh = why::Mesh::CreateCube();

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

    auto gun = why::GameObject::LoadGLTF("Models/sten_gunmachine_carbine/scene.gltf");
    gun->SetParent(camera);
    gun->SetPosition(glm::vec3(0.75f, -0.5f, -0.75f));
    gun->SetScale(glm::vec3(-1.0f, 1.0f, 1.0f));

    if (auto anim = gun->GetComponent<why::AnimationComponent>())
    {
        if (auto bullet = gun->FindChildByName("bullet_33"))
        {
            bullet->SetActive(false);
        }

        if (auto fire = gun->FindChildByName("BOOM_35"))
        {
            fire->SetActive(false);
        }

        anim->Play("shoot", false);
        //anim->Play("shoot"); ²¥·Å
    }

    auto light = m_scene.CreateObject("Light");
    auto lightComp = new why::LightComponent();
    lightComp->SetColor(glm::vec3(1.0f));
    light->AddComponent(lightComp);
    light->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));

    
    return true;
}

void Game::Update(float deltaTime)
{
    m_scene.Update(deltaTime);
}

void Game::Destroy()
{

}


