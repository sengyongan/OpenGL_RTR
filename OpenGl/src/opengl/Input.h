#pragma once
//用户输入

#include"hzpch.h"

namespace Opengl {
	class Input
	{
    public:
        static bool IsKeyPressed(int keycode);//是否按下键盘
        static bool IsMouseButtonPressed(int button);//是否按下鼠标
        static std::pair<float, float>GetMousePosition();//获取鼠标位置
        static float GetMouseX();//获取鼠标x
        static float GetMouseY();

	};

}
