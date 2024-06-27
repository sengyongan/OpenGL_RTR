#pragma once
//纹理
#include<glad/glad.h>
#include<string>
#include<vector>

using namespace std;

namespace Opengl {
	class Texture {
    public:
        Texture() {};
		Texture(const string& path);//创建一个纹理
		~Texture();

        uint32_t GetWidth() const  { return m_Width; }
        uint32_t GetHeight() const  { return m_Height; }
        uint32_t GetRendererID() const  { return m_RendererID; }
        unsigned int GetCubeTexture() const  { return m_CubeTextureID; }

        const string& GetPath() const  { return m_Path; }

        void SetData(void* data, uint32_t size = 0) ;

        void Bind(uint32_t slot = 0) const ;
        void BindCubeTexture(uint32_t slot = 0) const ;

        bool IsLoaded() const  { return m_IsLoaded; }

        static unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false) ;//创建模型纹理
        unsigned int loadCubemap(vector<std::string> path);//创建立方体贴图
        


    private:
        string m_Path;//路径

        bool m_IsLoaded = false;

        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;
        uint32_t m_CubeTextureID;

        GLenum  m_DataFormat;//格式

	};
}
