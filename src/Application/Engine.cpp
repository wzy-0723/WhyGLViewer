#include "Engine.h"
#include "Game.h"
#include "GameObject.h"
#include "Component.h"
#include "CameraComponent.h"
#include "OpenGLFunc.h"
#include "Logger.h"
#include "FileUtil.h"
#include "Common.h"

namespace why
{
	namespace
	{
		std::unique_ptr<Logger> g_loggerPtr;
		int32_t g_nLogLevel = -1;
	}

	void windowSizeCallback(int width, int height)
	{		
		SINGLETON_PTR(GraphicsAPI)->SetViewport(0, 0, width, height);
	}

	bool Engine::Init(int width, int height)
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
		SINGLETON_PTR(GraphicsAPI)->SetViewport(0, 0, width, height);

		m_physicsManager.Init();
		m_audioManager.Init();
		m_pRederQueue = std::make_unique<why::RenderQueue>();
		m_pRederQueue->Init();
		m_fontManager.Init();

		m_pApplication = std::make_unique<Game>();
		m_pInputManager = std::make_unique<why::InputManager>();
				


		Scene::RegisterTypes();
		m_pApplication->RegisterTypes();

		return m_pApplication->Init();
	}

	AudioManager& Engine::GetAudioManager()
	{
		return m_audioManager;
	}

	FontManager& Engine::GetFontManager()
	{
		return m_fontManager;
	}

	void Engine::Run()
	{
		SINGLETON_PTR(why::GraphicsAPI)->SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		SINGLETON_PTR(why::GraphicsAPI)->ClearBuffers();

		auto now = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float>(now - m_lastTimePoint).count();
		m_lastTimePoint = now;

		m_physicsManager.Update(deltaTime);
		// 先更新数据，再提交绘制
		m_pApplication->Update(deltaTime);

		CameraData cameraData;
		std::vector<LightData> lights;


		GLint viewport[4];
		OPENGLFUNC->glGetIntegerv(GL_VIEWPORT, viewport);
		float aspect = static_cast<float>(viewport[2]) / static_cast<float>(viewport[3]);

		int width = viewport[2];
		int height = viewport[3];

		if (m_pCurrentScene)
		{
			if (auto cameraObject = m_pCurrentScene->GetMainCamera())
			{
				// logic for matrices
				auto cameraComponent = cameraObject->GetComponent<CameraComponent>();
				if (cameraComponent)
				{					
					cameraData.viewMatrix = cameraComponent->GetViewMatrix();//@why.tostudy:相机变化
					cameraData.projectionMatrix = cameraComponent->GetProjectionMatrix(aspect);
					cameraData.position = cameraObject->GetWorldPosition();
					/*
						生成正交投影矩阵（正射投影）。
						正交投影特点：没有透视近大远小，平行线永远平行。
					*/
					cameraData.orthoMatrix = glm::ortho(
						0.0f, static_cast<float>(width),
						0.0f, static_cast<float>(height)
					);
				}
			}
			lights = m_pCurrentScene->CollectLights();
		}

		m_pRederQueue->Draw(cameraData, lights);
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