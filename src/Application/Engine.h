#pragma once
#include "Singleton.h"
#include "Application.h"
#include "InputManager.h"

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

    private:
        Engine() {};

    private:
        std::unique_ptr<why::InputManager>      m_pInputManager;
        std::unique_ptr<why::Application>       m_pApplication;
        std::unique_ptr<why::RenderQueue>       m_pRederQueue;

        std::chrono::steady_clock::time_point   m_lastTimePoint;
    };
}