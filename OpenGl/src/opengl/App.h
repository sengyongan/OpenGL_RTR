#pragma once
//作为应用API

#include "core.h"
#include"Window.h"


namespace Opengl {
	
	class BUILD_API App {
	public:
		App();
		~App();
		void Run();

		//App* Get() { return s_Instance; }
	private:
		//类对象
		Window m_Window;

	};
}