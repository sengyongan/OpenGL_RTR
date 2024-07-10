#pragma once
//帧缓冲
#include"hzpch.h"
#include"opengl/core/App.h"
namespace Opengl {

    struct FramebufferSpecification {
        uint32_t Width, Height;
        uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };

    class Framebuffer {
    public:
        Framebuffer(const FramebufferSpecification& spec);
        ~Framebuffer();
        //init
        void initMultisampleAttachment();//MSAA(抗锯齿）
        void initColorAttachment();//
        void iniDepthAttachment();//
        void initDepthCubeAttachment();//
        //bind ID
        void BindMultisample();
        void BindRendererID();
        void BindDepthRendererID();
        void BindDepthCubeRendererID();
        //bind Attach
        void BindMultisampleTexture();
        void BindTexture();
        //
        void Unbind();
        //移动buffer
        void BlitFramebuffer();

        //Get
        uint32_t GetMultisampleRendererID() const { return m_MultisampleRendererID; };//获取颜色缓冲附件

        uint32_t GetRendererID() const { return m_RendererID; };//获取颜色缓冲附件
        uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; };//获取颜色缓冲附件

        uint32_t GetDepthRendererID() const { return m_DepthRendererID; };//获取颜色缓冲
        uint32_t GetDepthAttachmentRendererID() const { return m_DepthMapAttachment; };//获取深度缓冲附件 

        uint32_t GetDepthCubeRendererID() const { return m_DepthCubeRendererID; };//获取颜色缓冲
        uint32_t GetDepthCubeAttachmentRendererID() const { return m_DepthMapCubeAttachment; };//获取深度缓冲附件 

        //
        const FramebufferSpecification& GetSpecification() const { return m_Specification; };
        //调整size
        void framebuffer_size();


    public:
        uint32_t m_MultisampleRendererID;//帧缓冲id---多重渲染
        uint32_t m_MultisampleColorAttachment;//附件id

        uint32_t m_RendererID;//帧缓冲id---颜色附件
        uint32_t m_ColorAttachment;//附件id
        uint32_t m_DepthAttachment;//附件id

        uint32_t m_DepthRendererID;//帧缓冲id---深度附件
        uint32_t m_DepthMapAttachment;//深度附件

        uint32_t m_DepthCubeRendererID;//帧缓冲id---深度立方体附件
        uint32_t m_DepthMapCubeAttachment;//深度附件


        FramebufferSpecification m_Specification;//帧缓冲格式
    };
}