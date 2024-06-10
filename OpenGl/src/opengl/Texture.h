#pragma once
//纹理
#include<string>
#include<glad/glad.h>

namespace Opengl {
	class Texture {
    public:
		Texture(const std::string& path);
		~Texture();

        uint32_t GetWidth() const  { return m_Width; }
        uint32_t GetHeight() const  { return m_Height; }
        uint32_t GetRendererID() const  { return m_RendererID; }

        const std::string& GetPath() const  { return m_Path; }

        void SetData(void* data, uint32_t size = 0) ;

        void Bind(uint32_t slot = 0) const ;

        bool IsLoaded() const  { return m_IsLoaded; }

    private:
        std::string m_Path;//路径

        bool m_IsLoaded = false;

        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;

        GLenum  m_DataFormat;//格式

	};
}
