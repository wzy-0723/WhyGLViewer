#include "Game.h"
#include <iostream>
#include "RenderQueue.h"
#include "TestObject.h"
namespace why
{
    bool Game::Init()
    {     
        m_scene.CreateObject<TestObject>("TestObject");
        return true;
    }

    void Game::Update(float deltaTime)
    {
        m_scene.Update(deltaTime);
    }

    void Game::Destroy()
    {

    }
}

