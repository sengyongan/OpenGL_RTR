#include"Framebuffer.h"
#include<glad/glad.h>
#include"Renderer.h"
namespace Opengl {
	Framebuffer::Framebuffer(const FramebufferSpecification& spec)
        : m_Specification(spec)
    {

        initMultisampleAttachment();
        initColorAttachment();
        iniDepthAttachment();
    }
    Framebuffer::~Framebuffer()
    {
        //glDeleteFramebuffers(1, &m_MultisampleRendererID);
    }
    void Framebuffer::initMultisampleAttachment()
    {

        glGenFramebuffers(1, &m_MultisampleRendererID);//创建帧缓冲
        glBindFramebuffer(GL_FRAMEBUFFER, m_MultisampleRendererID);
        ///ColorAttachment////////////////////////////////////////////////////////////////////////////////
        glGenTextures(1, &m_MultisampleColorAttachment);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_MultisampleColorAttachment);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, m_Specification.Width, m_Specification.Height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_MultisampleColorAttachment, 0);

        ///rendererBufferObj///////////////////////////////////////////////////////////////////////////////
        uint32_t rendererBufferObj;
        glGenRenderbuffers(1, &rendererBufferObj);
        glBindRenderbuffer(GL_RENDERBUFFER, rendererBufferObj);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rendererBufferObj);


        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        //
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void Framebuffer::initColorAttachment()
    {
        // configure second post-processing framebuffer
        glGenFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        // create a color attachment texture
        glGenTextures(1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Specification.Width, m_Specification.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);	// we only need a color buffer

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void Framebuffer::iniDepthAttachment()
    {
        //Gen
        glGenFramebuffers(1, &m_DepthRendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_DepthRendererID);
        //深度纹理附件
        glGenTextures(1, &m_DepthMapAttachment);
        glBindTexture(GL_TEXTURE_2D, m_DepthMapAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
            m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        //设置纹理参数
        float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);//允许开发者自定义纹理行为
        //附加到帧缓冲
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMapAttachment, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //Check
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
    void Framebuffer::BindMultisample()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_MultisampleRendererID);

    }
    void Framebuffer::BindRendererID()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    }
    void Framebuffer::BindDepthRendererID()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_DepthRendererID);
    }
    void Framebuffer::BindMultisampleTexture()
    {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_MultisampleColorAttachment);
    }
    void Framebuffer::BindTexture()
    {
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
    }
    void Framebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
    void Framebuffer::BlitFramebuffer()
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MultisampleRendererID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RendererID);
        glBlitFramebuffer(0, 0, m_Specification.Width, m_Specification.Height, 0, 0, m_Specification.Width, m_Specification.Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    }
    void Framebuffer::framebuffer_size()
    {
        if ((m_Specification.Width == App::Get().GetWindow().GetNewWidth() &&
            m_Specification.Height == App::Get().GetWindow().GetNewHeight())
            ||
            (App::Get().GetWindow().GetNewWidth() < 0 || App::Get().GetWindow().GetNewHeight() < 0))
        {
            return;
        }
        m_Specification.Width = App::Get().GetWindow().GetNewWidth();
        m_Specification.Height = App::Get().GetWindow().GetNewHeight();

        initMultisampleAttachment();
        initColorAttachment();
        iniDepthAttachment();

    }
}