#pragma once
//帧缓冲
#include"hzpch.h"
#include"opengl/core/App.h"
namespace Opengl {
    enum class FramebufferTextureFormat//帧缓冲纹理格式
    {
        None = 0,

        // Color//颜色
        RGBA16F,
        RGBA,//红色整形

        // Depth/stencil//深度/模板
        DEPTH24STENCIL8,

        // Defaults
        Depth = DEPTH24STENCIL8//depth的默认值
    };

    struct FramebufferTextureSpecification//帧缓冲纹理规范
    {
        FramebufferTextureSpecification() = default;
        FramebufferTextureSpecification(FramebufferTextureFormat format)//格式
            : TextureFormat(format) {}

        FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;//纹理格式变量
        // TODO: filtering/wrap
    };

    struct FramebufferAttachmentSpecification//帧缓冲器附件规范
    {
        FramebufferAttachmentSpecification() = default;
        FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)//纹理规范(接受列表）
            : Attachments(attachments) {}

        std::vector<FramebufferTextureSpecification> Attachments;//数组类型
    };

    struct FramebufferSpecification {//帧缓冲器规范
        uint32_t Width, Height;
        uint32_t Samples = 1;//多重采样
        FramebufferAttachmentSpecification Attachments;//附件规范-
        bool SwapChainTarget = false;//交换链目标
    };

    class Framebuffer {
    public:
        Framebuffer(const FramebufferSpecification& spec);
        ~Framebuffer();

        //init
        void initMultisampleAttachment();//MSAA(抗锯齿）多重采样
        void initColorAttachment();//普通的颜色纹理附件，深度渲染对象附件
        void iniDepthAttachment();//深度附件，平行光阴影
        void initDepthCubeAttachment();//深度立方体附件，点阴影
        void InvalidateMRT();//MRT
        void Initpingpong();//pingpong
        void InitSSAO();//pingpong

        //bind ID
        void BindMultisample();
        void BindRendererID();
        void BindDepthRendererID();
        void BindDepthCubeRendererID();
        //bind Attach
        void BindMultisampleTexture();
        void BindTexture();
        //
        void BindMRTFramebuffer();
        void BindPingPongFramebuffer(const int index);
        //
        void BindSSAOFramebuffer();
        void BindSSAOBlurFramebuffer();
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

        uint32_t GetMRTRendererID() const { return m_MRTRendererID; };//获取颜色缓冲
        uint32_t GetMRTAttachmentRendererID(const int index) const { return colorBuffers[index]; };//获取颜色缓冲
        //
        uint32_t GetPingPongRendererID(const int index) const { return pingpongFBO[index]; };//获取颜色缓冲
        uint32_t GetPingPongAttachmentRendererID(const int index) const { return pingpongColorbuffers[index]; };//获取颜色缓冲
        //
        uint32_t GetSSAOColorBufferAttachmentID() const { return ssaoColorBuffer; };//获取颜色缓冲
        uint32_t GetSSAOColorBufferBlurAttachmentID() const { return ssaoColorBufferBlur; };//获取颜色缓冲

        //
        const FramebufferSpecification& GetSpecification() const { return m_Specification; };
        //调整size
        void framebuffer_size();

    public:
        //MSAA////////////////////////////////////////////////////////////
        uint32_t m_MultisampleRendererID;//帧缓冲id---多重渲染
        uint32_t m_MultisampleColorAttachment;//附件id

        //普通////////////////////////////////////////////////////////////
        uint32_t m_RendererID;//帧缓冲id---颜色附件
        uint32_t m_ColorAttachment;//附件id
        uint32_t m_DepthAttachment;//附件id

        //点阴影////////////////////////////////////////////////////////////
        uint32_t m_DepthRendererID;//帧缓冲id---深度附件
        uint32_t m_DepthMapAttachment;//深度附件

        //立方体////////////////////////////////////////////////////////////
        uint32_t m_DepthCubeRendererID;//帧缓冲id---深度立方体附件
        uint32_t m_DepthMapCubeAttachment;//深度附件

        //pingpong////////////////////////////////////////////////////////////
        uint32_t pingpongFBO[2];
        uint32_t pingpongColorbuffers[2];
        //SSAO////////////////////////////////////////////////////////////
        uint32_t ssaoFBO, ssaoBlurFBO;
        uint32_t ssaoColorBuffer, ssaoColorBufferBlur;

        /////////////////////////////////////////////////////////////////////
        FramebufferSpecification m_Specification;//帧缓冲格式


        //多渲染目标MRT
        uint32_t m_MRTRendererID;//帧缓冲id---深度立方体附件
        //纹理附件规范（格式）
        std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;//帧缓冲器颜色附件规范数组（可以有多个）
        FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;////帧缓冲器深度和模板附件规范////默认移动赋值操作符（将FramebufferTextureSpecification中的TextureFormat设置为format.TextureFormat
        //附件id
        unsigned int colorBuffers[10];
        uint32_t m_MRTDepthAttachment = 0;


    };
}