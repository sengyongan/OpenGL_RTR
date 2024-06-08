#pragma once
//着色器
#include "core.h"

#include <string>

namespace  Opengl {
	class  Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

	private:
		unsigned int m_RendererID;//着色器程序

	};

}

