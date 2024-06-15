#pragma once
#include"Shader.h"
#include"hzpch.h"
#include"VertexArray.h"
#include"Renderer.h"

namespace Opengl {
	class Draw
	{
	public:
		virtual ~Draw() {}

		virtual void Bind() const = 0;//设置VAO（VBO，EBO）
		virtual void OnDraw(const std::shared_ptr<Shader>&  shader) const = 0;//绑定VAO，绘制(对shared_ptr类型对象的引用）
	};


}