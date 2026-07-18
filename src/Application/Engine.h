#pragma once
#include "Singleton.h"
#include "Application.h"
#include "InputManager.h"
#include "Scene.h"
#include "FileSystem.h"
namespace why
{
    class Engine
    {
        template<typename> friend class Singleton;
    public:

        bool Init();
        void Run();
        void Destroy();

        Application* GetApplication() { return m_pApplication.get(); };
        InputManager* GetInputManager() { return m_pInputManager.get(); };
        RenderQueue* GetRenderQueue() { return m_pRederQueue.get(); };
        FileSystem& GetFileSystem() { return m_fileSystem; };

        void SetScene(Scene* scene) { m_pCurrentScene.reset(scene); };
        Scene* GetScene() { return m_pCurrentScene.get(); };

    private:
        Engine() {};

    private:
        std::chrono::steady_clock::time_point   m_lastTimePoint;

        std::unique_ptr<InputManager>           m_pInputManager;
        std::unique_ptr<Application>            m_pApplication;
        std::unique_ptr<RenderQueue>            m_pRederQueue;
        FileSystem m_fileSystem;

        std::unique_ptr<Scene>                  m_pCurrentScene;
    };
}