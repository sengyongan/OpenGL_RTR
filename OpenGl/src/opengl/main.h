#pragma once
//Èë¿Úµã
#include"App.h"

Opengl::App* app = new Opengl::App();
//Opengl::App* s_Instance = app.Get();

//extern Opengl::App*  Opengl::createApp();
//int main(int argc, char** argv){
//	auto app = Opengl::createApp();
//	app->Run();
//	delete app;
//}
//extern Opengl::App* Opengl::App::s_Instance;
int main(int argc, char** argv) {
	app->Run();
	delete app;
}
