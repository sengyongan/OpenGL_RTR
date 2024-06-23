#include"hzpch.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Opengl {
	Texture::Texture(const std::string& path)
		: m_Path(path)
	{
		// load and create a m_RendererID 
		stbi_set_flip_vertically_on_load(true);
		// -------------------------
		int width, height, channels;//加载图像文件，并将图像数据存储在data指针中，获取图像的宽度、高度和通道数。
			
		glGenTextures(1, &m_RendererID);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);//可以用来读取图像文件并将其解码为像素数据。
		
		if(data)
		{
			m_IsLoaded = true;

			m_Width = width;
			m_Height = height;
			//channels
			GLenum  dataFormat = 0;//Format格式
			if (channels == 1) {//图片有透明通道
				dataFormat = GL_RED;
			}
			else if (channels == 3) {
				dataFormat = GL_RGB;
			}
			else if (channels == 4) {
				dataFormat = GL_RGBA;
			}
			m_DataFormat = dataFormat;

			////////////////////////////////
			glBindTexture(GL_TEXTURE_2D, m_RendererID); // all upcoming GL_TEXTURE_2D operations now have effect on this m_RendererID object

			// set the m_RendererID wrapping parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// set m_RendererID filtering parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//为绑定的纹理对象创建纹理
			//(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
			
			glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}
	Texture::~Texture()
	{
		glDeleteTextures(1, &m_RendererID);
	}
	void Texture::Bind(uint32_t slot) const
	{
		glBindTexture(GL_TEXTURE_2D, m_RendererID);//solt在OpenGLTexture2D重写中初始为0

	}

	void Texture::BindCubeTexture(uint32_t slot) const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTextureID);
	}

	unsigned int Texture::TextureFromFile(const char* path, const std::string& directory, bool gamma)
	{
		stbi_set_flip_vertically_on_load(true);
		string filename = string(path);
		filename = directory + '/' + filename;//放在同一个文件中//obj前面的路径+当前的名字
		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
			//std::cout << "load at path: " << path << std::endl;

		}
		else
		{
			std::cout << "Model_Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}
	unsigned int Texture::loadCubemap(vector<std::string> faces)
	{
		stbi_set_flip_vertically_on_load(false);

		glGenTextures(1, &m_CubeTextureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTextureID);

		int width, height, nrComponents;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return m_CubeTextureID;
	}
}
