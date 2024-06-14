#pragma once
//作为应用API

#include "core.h"
#include"Window.h"
#include"EditorCamera.h"

namespace Opengl {
	
	class BUILD_API App {
	public:
		App();
		~App();
		void Run();

		static App& Get() { return *s_Instance; }
		Window& GetWindow() { return  *m_Window; }//返回类对象指针的引用，防止发生拷贝
		EditorCamera& GetCamera() { return *m_EditorCamera; }

	private:
		//类对象
		static App* s_Instance;//唯一的应用程序单例

		std::unique_ptr<Window> m_Window;//

		std::unique_ptr<EditorCamera> m_EditorCamera;

		//Window* m_Window;

	};
}