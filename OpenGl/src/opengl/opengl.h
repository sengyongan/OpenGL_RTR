#pragma once
#include"core.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace Opengl {

	class  BUILD_API opengl {
	public:
		void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		void processInput(GLFWwindow* window);

	};
}