#include"hzpch.h"
#include "Shader.h"

#include <glad/glad.h>

namespace Opengl {
	Shader::Shader(const char* vertexPath, const char* fragmentPath) {
		//////////////////////////////////////////////////////////////////////////////////
		///分解文件/////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////

		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;//保存从文件读取的字符串
		std::string fragmentCode;

		std::ifstream vShaderFile;//输入流
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
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
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		}
		//转换类型
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		//////////////////////////////////////////////////////////////////////////////////
		///创建链接着色器/////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////
		//顶点着色器
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexShader, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader);
		checkCompileErrors(vertexShader, "VERTEX");
		// check for shader compile errors
		//int success;
		//char infoLog[512];

		////检查顶点着色器
		//glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		//if (!success)
		//{
		//	glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		//	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		//}
		 //Create an empty fragment shader handle片段着色器
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		
		glShaderSource(fragmentShader, 1, &fShaderCode, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		checkCompileErrors(fragmentShader, "FRAGMENT");
		// check for shader compile errors
		//glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		//if (!success)
		//{
		//	glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		//	std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		//}


		// link shaders着色器程序
		m_RendererID =  glCreateProgram();
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);
		glLinkProgram(m_RendererID);
		checkCompileErrors(m_RendererID, "PROGRAM");
		// check for linking errors
		//glGetProgramiv(m_RendererID, GL_LINK_STATUS, &success);
		//if (!success) {
		//	glGetProgramInfoLog(m_RendererID, 512, NULL, infoLog);
		//	std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		//}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
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

	void Shader::checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")//非着色器程序
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else//着色器程序
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
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