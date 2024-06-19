#include "opengl/core/App.h"

#include"opengl/renderer/Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Opengl {
	const unsigned int SCR_WIDTH = 1600;
	const unsigned int SCR_HEIGHT = 900;

	// timing
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	App* App::s_Instance = nullptr;

	App::App()
	{
		//init
		s_Instance = this;

		m_Window = std::make_unique<Window>();
		m_Window->Init(SCR_WIDTH, SCR_HEIGHT, "senlongan_OpenGL_demo");
		Renderer::init();
		m_EditorCamera = std::make_unique<EditorCamera>(30.0f, 1.778f, 0.1f, 1000.0f);

		
	}
	App::~App()
	{
		m_Window->~Window();
	}
	void App::Run()
	{
		while(!glfwWindowShouldClose(m_Window->GetNativeWindow())){
			// ts
			float currentFrame = static_cast<float>(glfwGetTime());//获取当前时间
			deltaTime = currentFrame - lastFrame;//时差（=当前 - 上一帧）
			lastFrame = currentFrame;//上一帧

			// ------
			//Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			//Renderer::Clear();
			if (m_EditorCamera->GetViewportWidth() != App::Get().GetWindow().GetNewWidth() &&
				m_EditorCamera->GetViewportHeight() != App::Get().GetWindow().GetNewHeight() &&
				(App::Get().GetWindow().GetNewWidth() > 0 && App::Get().GetWindow().GetNewHeight() > 0)) 
			{
				m_EditorCamera->SetViewportSize(App::Get().GetWindow().GetNewWidth(), App::Get().GetWindow().GetNewHeight());
			}
			m_EditorCamera->OnUpdate(deltaTime);
			Renderer::BeginScene(*m_EditorCamera);
			Renderer::EndScene();
			//
			m_Window->OnUpdate();
		}
	}
}