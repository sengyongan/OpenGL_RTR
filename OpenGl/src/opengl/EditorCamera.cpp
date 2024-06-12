#include "hzpch.h"
#include "EditorCamera.h"

#include"Input.h"
#include"App.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Opengl {
    // camera
    // timing
    //float deltaTime = 0.0f;	// time between current frame and last frame
    //float lastFrame = 0.0f;

    EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
        : m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip),
          m_ProMatrix(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
    {
        glfwSetScrollCallback(App::Get().GetWindow().GetNativeWindow(), MouseScrolledEvent);
        UpdateView();
    }

    void EditorCamera::UpdateProjection()//重新更新投影矩阵
    {
        m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
        m_ProMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
    }

    void EditorCamera::UpdateView()//更新视口位置
    {

        m_Position = CalculatePosition();//获取位置

        glm::quat orientation = GetOrientation();//视图矩阵表示：摄像机的位置旋转和缩放
        m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);//四元数方向转化为矩阵
        m_ViewMatrix = glm::inverse(m_ViewMatrix);//翻转，改变场景的绘制
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::pair<float, float> EditorCamera::PanSpeed() const//被视口大小影响速度
    {
        float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f最大限制，视口越小，移动越慢
        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return { xFactor, yFactor };
    }

    float EditorCamera::RotationSpeed() const
    {
        return 0.8f;
    }

    float EditorCamera::ZoomSpeed() const
    {
        float distance = m_Distance * 0.2f;
        distance = std::max(distance, 0.0f);//距离最小为0
        float speed = distance * distance;//平方函数：距离越大，移动越快，距离越小移动越慢
        speed = std::min(speed, 100.0f); // 速度最大为100
        return speed;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void EditorCamera::OnUpdate(float deltaTime)
    {
        // ts
        // --------------------
        //
        if (Input::IsKeyPressed(GLFW_KEY_LEFT_ALT))//按住alt
        {
            //获取鼠标移动差值
            const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };//获取鼠标相对于窗口位置
            glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;//计算鼠标移动距离（求差），在2D屏幕上
            m_InitialMousePosition = mouse;//上一次的位置
            
            //
            if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))//鼠标中间平移（//速度根据视口大小&&距离改变）
                MousePan(delta);
            else if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))//鼠标左键旋转（速度不变）
                MouseRotate(delta);
            else if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))//鼠标右键缩放
                MouseZoom(delta.y);

        }

        //
        float cameraSpeed = static_cast<float>(2.5 * deltaTime);
        auto [xSpeed, ySpeed] = PanSpeed();//获取平移速度

        if (Input::IsKeyPressed(GLFW_KEY_W))
            m_FocalPoint +=  GetUpDirection() * cameraSpeed * xSpeed * m_Distance;//标量*向量，再用向量m_FocalPoint + 移动向量
        if (Input::IsKeyPressed(GLFW_KEY_S))
            m_FocalPoint -=  GetUpDirection() * cameraSpeed * xSpeed * m_Distance;
        if (Input::IsKeyPressed(GLFW_KEY_A))
            m_FocalPoint -= GetRightDirection() * cameraSpeed * ySpeed * m_Distance;//点乘，正规化为-1——1
        if (Input::IsKeyPressed(GLFW_KEY_D))
            m_FocalPoint += GetRightDirection() * cameraSpeed * ySpeed * m_Distance;

        //
        UpdateView();//更新m_ViewMatrix
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void EditorCamera::MouseScrolledEvent(GLFWwindow* window, double xOffset, double yOffset)
    {
        double delta = yOffset * 0.1f;
        App::Get().GetCamera().MouseZoom(delta);
        App::Get().GetCamera().UpdateView();
    }

    void EditorCamera::MousePan(const glm::vec2& delta)//传入鼠标的差
    {
        auto [xSpeed, ySpeed] = PanSpeed();//获取平移速度
        m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;//vec2（x,y)，鼠标右移差值为正，想要摄像机向左移
        m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;//鼠标上移差值为负
        //std::cout << delta.x << std::endl;
        //std::cout << delta.y << std::endl;
    }

    void EditorCamera::MouseRotate(const glm::vec2& delta)
    {
        //float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;//决定旋转方向
        m_Pitch += delta.y * RotationSpeed();//鼠标上移差值为负，想要相机局部坐标向上旋转，向上是负值
        m_Yaw +=  delta.x * RotationSpeed();//鼠标右移差值为正，想要相机局部坐标向右旋转，向右是正数
        //std::cout << m_Pitch << m_Yaw <<std::endl;
        //std::cout << m_Yaw << std::endl;

    }

    void EditorCamera::MouseZoom(float delta)//delta.y，对于缩放，改变m_Position实际的最后结果
    {
        m_Distance -= delta * ZoomSpeed();//鼠标上移值越小，-= 越远
        if (m_Distance < 1.0f)
        {
            m_FocalPoint += GetForwardDirection();
            std::cout << m_Distance <<std::endl;
            m_Distance = 1.0f;
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    glm::vec3 EditorCamera::GetUpDirection() const//返回形式vec3(0.0, -m_Yaw，0.0）
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetRightDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetForwardDirection() const
    {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }
    //
    glm::vec3 EditorCamera::CalculatePosition() const//计算出最终的新位置向量
    {   //-是因为，距离越远，值越大，想要后移为-z， 应取反
        return m_FocalPoint - GetForwardDirection() * m_Distance;//新的位置 = 平移的目标点 - 在相机的z方向上的距离
    }

    glm::quat EditorCamera::GetOrientation() const//由旋转决定
    {
        return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));//转化为4元数
    }

}