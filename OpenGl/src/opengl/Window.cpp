#include "Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Opengl {
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;


    Window::Window()
    {
    }

    Window::~Window()
    {
        Window::Ondestory();
    }
    void Window::Init()
    {
        // glfw: initialize and configure初始化glfw
        // ------------------------------
        glfwInit();
        // glfw window creation创建窗口
        // --------------------
        m_Window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "senlongan_OpenGL_demo", NULL, NULL);
        if (m_Window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();//终止使用 OpenGL 或 Vulkan 的应用程序
        }
        glfwMakeContextCurrent(m_Window);//传入窗口上下文
        //callbake
        glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);//设置回调函数，可以调整窗口大小
        processInput(m_Window);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }

    }
    void Window::OnUpdate()
    {
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        //if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        //    glfwSetWindowShouldClose(m_Window, true);

        glfwSwapBuffers(m_Window);
        glfwPollEvents();//检测callback事件
    }

    void Window::Ondestory()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();

    }

    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void Window::processInput(GLFWwindow* window)
    {
        //isRunning = false;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }


    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }


}
