#pragma once
//Ö¡»º³å
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

        void initMultisampleAttachment();//MSAA(¿¹¾â³Ý£©
        void initColorAttachment();//ÆÕÍ¨framebuffer

        void BindMultisample();
        void BindRendererID();

        void BindMultisampleTexture();
        void BindTexture();

        void Unbind();

        void BlitFramebuffer();

        uint32_t GetMultisampleRendererID() const { return m_MultisampleRendererID; };//»ñÈ¡ÑÕÉ«»º³å¸½¼þ

        uint32_t GetRendererID() const { return m_RendererID; };//»ñÈ¡ÑÕÉ«»º³å¸½¼þ
        uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; };//»ñÈ¡ÑÕÉ«»º³å¸½¼þ

        const FramebufferSpecification& GetSpecification() const { return m_Specification; };


        void framebuffer_size();


    public:
        uint32_t m_MultisampleRendererID;//Ö¡»º³åid
        uint32_t m_MultisampleColorAttachment;//¸½¼þid

        uint32_t m_RendererID;//Ö¡»º³åid
        uint32_t m_ColorAttachment;//¸½¼þid

        FramebufferSpecification m_Specification;//Ö¡»º³å¸ñÊ½
    };
}