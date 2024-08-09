#include"hzpch.h"
#include "Shader.h"

#include <glad/glad.h>

namespace Opengl {
	Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath ) {
		//////////////////////////////////////////////////////////////////////////////////
		//分解文件/////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////

		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;//保存从文件读取的字符串
		std::string fragmentCode;
		std::string geometryCode;

		std::ifstream vShaderFile;//输入流
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try//将文件的字符流都读取到字符串中
		{
			// open files打开文件
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;//字符串流
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();//读写
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();//关闭文件
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();//字符串流保存到字符串
			fragmentCode = fShaderStream.str();
			// if geometry shader path is present, also load a geometry shader
			if (geometryPath != nullptr)//有可能不存在
			{
				gShaderFile.open(geometryPath);//打开文件
				std::stringstream gShaderStream;//字符串流
				gShaderStream << gShaderFile.rdbuf();//读写
				gShaderFile.close();//关闭文件
				geometryCode = gShaderStream.str();
			}
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		}
		//////////////////////////////////////////////////////////////////////////////////
		//创建shader/////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		//vertex着色器
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
		glCompileShader(vertexShader);
		checkCompileErrors(vertexShader, "VERTEX");
		 //Create an empty fragment shader handle片段着色器
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, 0);
		glCompileShader(fragmentShader);
		checkCompileErrors(fragmentShader, "FRAGMENT", vertexPath);
		//
		// if geometry shader is given, compile geometry shader
		unsigned int geometry;
		if (geometryPath != nullptr)
		{
			const char* gShaderCode = geometryCode.c_str();
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			checkCompileErrors(geometry, "GEOMETRY", fragmentPath);
		}
		//////////////////////////////////////////////////////////////////////////////////
		//link shaders program///////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		m_RendererID =  glCreateProgram();
		glAttachShader(m_RendererID, vertexShader);//附加
		glAttachShader(m_RendererID, fragmentShader);
		if (geometryPath != nullptr)
			glAttachShader(m_RendererID, geometry);
		glLinkProgram(m_RendererID);
		checkCompileErrors(m_RendererID, "PROGRAM", geometryPath);
		//delete
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (geometryPath != nullptr)
			glDeleteShader(geometry);
	}
	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::checkCompileErrors(unsigned int shader, std::string type, const char* vertexPath , const char* fragmentPath , const char* geometryPath )
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")//非着色器程序
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
				std::cout << vertexPath <<  "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else//着色器程序
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;

			}
		}
	}


	/////////////////////////////////////////////////////////////////////////////
	void Shader::SetInt(const std::string& name, int value)
	{
		UploadUniformInt(name, value);
	}

	void Shader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		UploadUniformIntArray(name, values, count);
	}

	void Shader::SetFloat(const std::string& name, float value)
	{
		UploadUniformFloat(name, value);
	}

	void Shader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		UploadUniformFloat2(name, value);
	}

	void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		UploadUniformFloat3(name, value);
	}

	void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		UploadUniformFloat4(name, value);
	}

	void Shader::SetMat3(const std::string& name, const glm::mat3& value)
	{
		UploadUniformMat3(name, value);
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		UploadUniformMat4(name, value);
	}

	void Shader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void Shader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void Shader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Shader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}



}