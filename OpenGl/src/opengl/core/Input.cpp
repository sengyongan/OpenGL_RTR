#include "Input.h"

#include"App.h"
#include<GLFW/glfw3.h>

namespace Opengl {
    
    bool Input::IsKeyPressed(int keycode)
    {   
        //Application实例--window类对象--m_Window窗口实例,//获取窗口,app->window class->m_window
        auto Window = static_cast<GLFWwindow*>(App::Get().GetWindow(). GetNativeWindow());
        auto state = glfwGetKey(Window, keycode);//获取建立的窗口，是否按压下按键
        return state == GLFW_PRESS;//按下成立返回真
    }
    bool Input::IsMouseButtonPressed(int button)
    {
        auto Window = static_cast<GLFWwindow*>(App::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(Window, button);
        return state == GLFW_PRESS;
    }
    std::pair<float, float> Input::GetMousePosition()//返回元组
    {
        auto Window = static_cast<GLFWwindow*>(App::Get().GetWindow().GetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(Window, &xpos, &ypos);//获取建立的窗口，鼠标的位置
        return { (float)xpos,(float)ypos };//通过glfw返回鼠标当前位置
    }
    float Input::GetMouseX()
    {
        auto [x, y] = GetMousePosition();
        return x;
    }
    float Input::GetMouseY()
    {
        auto [x, y] = GetMousePosition();
        return y;

    }

}