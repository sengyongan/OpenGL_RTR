#pragma once

#include"hzpch.h"
#include "core.h"
#include"opengl/renderer/Shader.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>


namespace Opengl {

	class BUILD_API TextRendering {

	public:
		TextRendering();
		void init();
		void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);

	public:
		FT_Library ft;
		FT_Face face;
		
		struct Character {
			GLuint     TextureID;  // 字形纹理的ID
			glm::ivec2 Size;       // 字形大小
			glm::ivec2 Bearing;    // 从基准线到字形左部/顶部的偏移值
			GLuint     Advance;    // 原点距下一个字形原点的距离
		};

		std::map<GLchar, Character> Characters;

		unsigned int VAO, VBO;
	};
}