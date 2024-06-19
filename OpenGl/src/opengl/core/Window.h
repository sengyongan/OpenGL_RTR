#pragma once
//负责管理窗口

#include"core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include<string>

namespace Opengl {


	class  Window {
	public:
		Window();
		~Window();
		void Init(const unsigned int& width, const unsigned int& height, std::string windowName );
		void OnUpdate();
		void Ondestory();
		
		GLFWwindow* GetNativeWindow() const { return m_Window; }
		const int& GetNewWidth() const { return m_NewWidth; }
		const int& GetNewHeight() const { return m_NewHeight; }

	public:
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void processInput(GLFWwindow* window);

	public:
		GLFWwindow* m_Window;//实际窗口

	private:
		int m_NewWidth, m_NewHeight;

	};
}