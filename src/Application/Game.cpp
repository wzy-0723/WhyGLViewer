#include "Game.h"
#include <iostream>
#include "RenderQueue.h"
#include "TestObject.h"
#include "Engine.h"
#include "CameraComponent.h"
#include "PlayerControllerComponent.h"
bool Game::Init()
{     

    auto camera = m_scene.CreateObject("Camera");
    camera->AddComponent(new why::CameraComponent());
    camera->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    camera->AddComponent(new why::PlayerControllerComponent());
    m_scene.SetMainCamera(camera);

    m_scene.CreateObject<TestObject>("TestObject");

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


