#pragma once
//负责管理窗口

#include"core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace Opengl {


	class  Window {
	public:
		Window();
		~Window();
		void Init();
		void OnUpdate();
		void Ondestory();
		
		GLFWwindow* GetNativeWindow() const { return m_Window; }


	public:
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void processInput(GLFWwindow* window);

	public:
		GLFWwindow* m_Window;//实际窗口

	};
}