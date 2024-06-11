#pragma once
//渲染
#include"VertexArray.h"
#include"EditorCamera.h"

#include <string>
#include <glm/glm.hpp>

namespace Opengl {
	class Renderer {
	public:
		static void BeginScene(const EditorCamera& camera);//因为渲染逻辑只存在一份，不需要多个实例
		static void init();
		static void EndScene();

		static void SetClearColor(const glm::vec4& color);
		static void Clear();

		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);

	};
}