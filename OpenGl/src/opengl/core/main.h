#pragma once
//Èë¿Úµã
#include"App.h"

Opengl::App* app = new Opengl::App();
//Opengl::App& app = Opengl::App::Get();

int main(int argc, char** argv) {
	app->Run();
	//app->~App();
	delete app;
}
