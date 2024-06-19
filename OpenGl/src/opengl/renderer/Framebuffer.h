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

        void Invalidate();

        void Bind();
        void BindTexture();
        void Unbind();


        uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; };//»ñÈ¡ÑÕÉ«»º³å¸½¼þ

        const FramebufferSpecification& GetSpecification() const { return m_Specification; };


        void framebuffer_size();


    public:
        uint32_t m_RendererID;//Ö¡»º³åid

        uint32_t m_ColorAttachment, m_DepthAttachment;//¸½¼þid

        FramebufferSpecification m_Specification;//Ö¡»º³å¸ñÊ½
    };
}