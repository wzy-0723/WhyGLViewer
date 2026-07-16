#include "Engine.h"
#include "Game.h"


namespace why
{
	bool Engine::Init()
	{
		m_pApplication = std::make_unique<why::Game>();
		m_pInputManager = std::make_unique<why::InputManager>();
		m_pRederQueue = std::make_unique<why::RenderQueue>();		

		return m_pApplication->Init();
	}

	void Engine::Run()
	{
		SINGLETON_PTR(why::GraphicsAPI)->SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		SINGLETON_PTR(why::GraphicsAPI)->ClearBuffers();

		auto now = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float>(now - m_lastTimePoint).count();
		m_lastTimePoint = now;

		// 先更新数据，再提交绘制
		m_pApplication->Update(deltaTime);
		m_pRederQueue->Draw();
	}

	void Engine::Destroy()
	{
		
	}
}