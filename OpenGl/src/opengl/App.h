#pragma once
//作为应用API

#include "core.h"
#include"Window.h"

#include"Shader.h"
#include"Buffer.h"
#include"opengl/Buffer_Abstraction.h"
#include"VertexArray.h"

namespace Opengl {
	
	class BUILD_API App {
	public:
		App();
		~App();
		void Run();

		App* Get() { return s_Instance; }
	private:

		static App* s_Instance;
		Window m_Window;//类对象

		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexArray> m_VertexArray;

		std::unique_ptr<Shader> m_BlueShader;
		std::unique_ptr<VertexArray> m_SquareVA;
	};
	//App* createApp();
}