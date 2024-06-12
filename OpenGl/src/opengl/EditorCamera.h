#pragma once
//透视摄像机
#include"Window.h"
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace Opengl {

    class EditorCamera 
    {
    public:

        EditorCamera() = default;
        EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);//传入到m_ProMatrix
        //
        void OnUpdate(float deltaTime);

        //位置
        inline float GetDistance() const { return m_Distance; }
        inline void SetDistance(float distance) { m_Distance = distance; }

        //设置视口大小
        inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }
        
        //Get
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& GetProjection() const { return m_ProMatrix; }
        glm::mat4 GetViewProjection() const { return m_ProMatrix * m_ViewMatrix; }//最终：获取视图投影矩阵(我们的编辑器相机）
        
        //获取相机经过旋转后，局部 上，右，前 方向轴
        glm::vec3 GetUpDirection() const;//获取上方向
        glm::vec3 GetRightDirection() const;
        glm::vec3 GetForwardDirection() const;

        //获取相机 位置 || 旋转四元数
        const glm::vec3& GetPosition() const { return m_Position; }
        glm::quat GetOrientation() const;

        //获取相机 绕某轴旋转的角度
        float GetPitch() const { return m_Pitch; }
        float GetYaw() const { return m_Yaw; }
        //
        static void MouseScrolledEvent(GLFWwindow* window, double xOffset, double yOffset);

    private:

        void UpdateProjection();//更新m_ProMatrix
        void UpdateView();//更新m_ViewMatrix（由相机的平移和旋转和缩放）决定

        //鼠标控制
        //bool OnMouseScroll(MouseScrolledEvent& e);

        //鼠标平移，旋转，缩放
        void MousePan(const glm::vec2& delta);
        void MouseRotate(const glm::vec2& delta);
        void MouseZoom(float delta);

        //计算位置
        glm::vec3 CalculatePosition() const;

        //鼠标平移，旋转，缩放的速度
        std::pair<float, float> PanSpeed() const;
        float RotationSpeed() const;
        float ZoomSpeed() const;

    private:
        //投影矩阵（视野，宽高比，近平面和远平面）
        float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

        //
        glm::mat4 m_ViewMatrix;//视图矩阵,
        glm::mat4 m_ProMatrix;//投影矩阵

        //
        glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };//计算后的新位置（平移的目标点 - 在相机的z方向上的距离）
        glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };//本次目标点

        //
        glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };//鼠标上一次位置

        //
        float m_Distance = 10.0f;//距离原点的距离
        //
        float m_Pitch = 0.0f, m_Yaw = 0.0f;//仰角和偏航角

        //
        float m_ViewportWidth = 1280, m_ViewportHeight = 720;//视口大小

    };

}