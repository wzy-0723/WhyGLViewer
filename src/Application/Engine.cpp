#include "Engine.h"
#include "Game.h"
#include "GameObject.h"
#include "Component.h"
#include "CameraComponent.h"
#include "OpenGLFunc.h"
#include "Logger.h"
#include "FileUtil.h"
namespace why
{
	namespace
	{
		std::unique_ptr<Logger> g_loggerPtr;
		int32_t g_nLogLevel = -1;
	}

	bool Engine::Init()
	{
		std::string strLog_dirPath = 
			SINGLETON_PTR(PathAppender)->SetSourcePath(GetExecutableDirectory()).AppendChildPath("log").GetPath();;

		g_loggerPtr.reset(new Logger(strLog_dirPath, "WhyGLViewer", g_nLogLevel));
		if (!CreateDirectories(strLog_dirPath))
		{
			LOG_ERROR << "create " << strLog_dirPath << " failed!!!";
			return false;
		}

		SINGLETON_PTR(GraphicsAPI)->Init();

		m_pApplication = std::make_unique<Game>();
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

		CameraData cameraData;
		int width = 0;
		int height = 0;
		GLint viewport[4];
		OPENGLFUNC->glGetIntegerv(GL_VIEWPORT, viewport);
		float aspect = static_cast<float>(viewport[2]) / static_cast<float>(viewport[3]);

		if (m_pCurrentScene)
		{
			if (auto cameraObject = m_pCurrentScene->GetMainCamera())
			{
				// logic for matrices
				auto cameraComponent = cameraObject->GetComponent<CameraComponent>();
				if (cameraComponent)
				{					
					cameraData.viewMatrix = cameraComponent->GetViewMatrix();//@why:相机变化
					cameraData.projectionMatrix = cameraComponent->GetProjectionMatrix(aspect);
				}
			}
		}

		m_pRederQueue->Draw(cameraData);
	}

	void Engine::Destroy()
	{
		if (m_pApplication)
		{
			m_pApplication->Destroy();
			m_pApplication.reset();
		}
	}
}