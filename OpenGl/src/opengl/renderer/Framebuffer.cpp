#include"Framebuffer.h"
#include<glad/glad.h>
#include"Renderer.h"
namespace Opengl {
    bool frist = true;
    namespace Utils//实用工具
    {
        //纹理目标
        static GLenum TextureTarget(bool multisampled)
        {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;//如果是多重采样，就为多重，否则返回普通
        }
        //创建纹理
        static void CreateTextures(uint32_t count, uint32_t* outID)//outID纹理标识符，传入数组的指针，count传入数组大小
        {   //glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
            glGenTextures( count, outID);
        }
        //绑定纹理
        static void BindTexture(bool multisampled, uint32_t id)
        {
            glBindTexture(TextureTarget(multisampled), id);
        }
        //颜色缓冲附件纹理
        static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
        {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                /////纹理对象分配空间
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
            }
            else if (frist) {
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, nullptr);//为纹理分配Data

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                frist = false;
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, nullptr);//为纹理分配Data

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            }
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);////将纹理对象附加到帧缓冲对象的 颜色附件点
        }
        //深度缓冲附件纹理
        static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
        {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, id);
        }
        //是否是深度格式
        static bool IsDepthFormat(FramebufferTextureFormat format)
        {
            switch (format)
            {
            case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
            }

            return false;
        }
        //将FramebufferTextureFormat中的格式转化为GL_开头的格式
        static GLenum HazelFBTextureFormatToGL(FramebufferTextureFormat format)
        {
            switch (format)
            {
            case FramebufferTextureFormat::RGBA16F: return GL_RGBA16F;
            case FramebufferTextureFormat::RGBA: return GL_RGBA;
            }

            return 0;
        }
    }

	Framebuffer::Framebuffer(const FramebufferSpecification& spec)
        : m_Specification(spec)
    {
        for (auto spec : m_Specification.Attachments.Attachments) {//纹理附件规范数组
            if (!Utils::IsDepthFormat(spec.TextureFormat))//不是深度缓冲
                m_ColorAttachmentSpecifications.emplace_back(spec);//添加到颜色缓冲格式数组
            else
                m_DepthAttachmentSpecification = spec;
        }
    }

    Framebuffer::Framebuffer()
    {
    }

    Framebuffer::~Framebuffer()
    {
        //glDeleteFramebuffers(1, &m_MultisampleRendererID);
    }
    void Framebuffer::InvalidateMRT()
    {
        glGenFramebuffers(1, &m_MRTRendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_MRTRendererID);
                // ---------------------------------------
        //unsigned int hdrFBO;
        bool multisample = m_Specification.Samples > 1;
        Utils::CreateTextures(sizeof(colorBuffers), colorBuffers);
        ////颜色格式存在
        if (m_ColorAttachmentSpecifications.size())
        {
            
            for (unsigned int i = 0; i < m_ColorAttachmentSpecifications.size(); i++)
            {
                Utils::BindTexture(multisample, colorBuffers[i]);
                switch (m_ColorAttachmentSpecifications[i].TextureFormat)//检查每个颜色各式
                {
                case FramebufferTextureFormat::RGBA16F:
                    Utils::AttachColorTexture(colorBuffers[i], m_Specification.Samples, GL_RGBA16F, GL_RGBA, 1024, 1024, i);
                    break;
                case FramebufferTextureFormat::RGBA:
                    Utils::AttachColorTexture(colorBuffers[i], m_Specification.Samples, GL_RGBA, GL_RGBA, 1024, 1024, i);
                    break;
                }
            }
        }
        //深度格式存在
        if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
        {
            glGenRenderbuffers(1, &m_MRTDepthAttachment);
            glBindRenderbuffer(GL_RENDERBUFFER, m_MRTDepthAttachment);

            switch (m_DepthAttachmentSpecification.TextureFormat)
            {
                case FramebufferTextureFormat::DEPTH24STENCIL8:
                    Utils::AttachDepthTexture(m_MRTDepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, 1024,1024);
                    break;
            }
        }
        //渲染到多个颜色缓冲
        if (m_ColorAttachmentSpecifications.size() > 1)
        {
            unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2 };
            glDrawBuffers(3, attachments);
        }

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
    void Framebuffer::Initpingpong()
    {
        glGenFramebuffers(2, pingpongFBO);
        glGenTextures(2, pingpongColorbuffers);
        for (unsigned int i = 0; i < 2; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
            glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1024, 1024, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
            // also check if framebuffers are complete (no need for depth buffer)
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                std::cout << "Framebuffer not complete!" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
    void Framebuffer::InitSSAO()
    {
        // SSAO color buffer
        glGenFramebuffers(1, &ssaoFBO);  
        glGenFramebuffers(1, &ssaoBlurFBO);

        glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
        glGenTextures(1, &ssaoColorBuffer);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1024, 1024, 0, GL_RED, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "SSAO Framebuffer not complete!" << std::endl;

        // and blur stage
        glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
        glGenTextures(1, &ssaoColorBufferBlur);
        glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1024, 1024, 0, GL_RED, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
    void Framebuffer::InitRender()
    {
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
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
        glGenFramebuffers(1, &m_RendererID);
        // - Create floating point color buffer
        glGenTextures(1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1024, 1024, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // - Create depth buffer (renderbuffer)
        GLuint rboDepth;
        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
        // - Attach buffers
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
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
    void Framebuffer::initDepthCubeAttachment()
    {  
        // configure depth map FBO
        // -----------------------
        glGenFramebuffers(1, &m_DepthCubeRendererID);
        // create depth cubemap texture
        glGenTextures(1, &m_DepthMapCubeAttachment);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_DepthMapCubeAttachment);
        for (unsigned int i = 0; i < 6; ++i)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 1024,1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_DepthCubeRendererID);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthMapCubeAttachment, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete4!" << std::endl;

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
    void Framebuffer::BindDepthCubeRendererID()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_DepthCubeRendererID);
    }
    void Framebuffer::BindMultisampleTexture()
    {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_MultisampleColorAttachment);
    }
    void Framebuffer::BindTexture()
    {
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
    }
    void Framebuffer::BindSSAOFramebuffer()
    {
        glBindTexture(GL_FRAMEBUFFER, ssaoFBO);
    }
    void Framebuffer::BindSSAOBlurFramebuffer()
    {
        glBindTexture(GL_FRAMEBUFFER, m_ColorAttachment);
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
    void Framebuffer::BindMRTFramebuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_MRTRendererID);
    }
    void Framebuffer::BindPingPongFramebuffer(int index)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[index]);
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