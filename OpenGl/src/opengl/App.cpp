#include "App.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Opengl {
	// settings

	//const char* vertexShaderSource = "#version 330 core\n"
	//	"layout (location = 0) in vec3 aPos;\n"
	//	"void main()\n"
	//	"{\n"
	//	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	//	"}\0";
	//const char* fragmentShaderSource = "#version 330 core\n"
	//	"out vec4 FragColor;\n"
	//	"void main()\n"
	//	"{\n"
	//	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	//	"}\n\0";
	std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";
	std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

	std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

	std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";
  //  float vertices[] = {
		// 0.5f,  0.5f, 0.0f,  // top right
		// 0.5f, -0.5f, 0.0f,  // bottom right
		//-0.5f, -0.5f, 0.0f,  // bottom left
		//-0.5f,  0.5f, 0.0f   // top left 
  //  };
	float vertices[3 * 7] = {//position + color
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};
	float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
	};
		
	//unsigned int indices[] = {  // note that we start from 0!
 //       0, 1, 3,  // first Triangle
 //       1, 2, 3   // second Triangle
 //   };

	unsigned int indices[3] = { 0, 1, 2 };

	unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

    App* App::s_Instance = nullptr;

	App::App()
	{
        s_Instance = this;

		//shader
        m_Shader.reset(new Shader(vertexSrc, fragmentSrc));//设置智能指针指向的对象
		m_BlueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
		///////////////////////////////////////////////////////////////////////
		/////Frist///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		m_VertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> vertexBuffer;//因为全局变量只需要VAO即可
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
					{ ShaderDataType::Float3, "a_Position" },
					{ ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		//进行布局
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		//m_IndexBuffer = std::make_unique<OpenGLIndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);
		///////////////////////////////////////////////////////////////////////
		/////Second///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////


		m_SquareVA = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});

		m_SquareVA->AddVertexBuffer(squareVB);

		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

		m_SquareVA->SetIndexBuffer(squareIB);
        // ------------------------------------------------------------------
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
	}
	App::~App()
	{
		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
        //glDeleteVertexArrays(1, &VAO);
        //glDeleteBuffers(1, &VBO);
        //glDeleteBuffers(1, &EBO);
        //glDeleteProgram(shaderProgram);
		m_Window.~Window();
		//glfwTerminate();

	}
	void App::Run()
	{
		while(!glfwWindowShouldClose(m_Window.m_Window)){

            // input
			if (glfwGetKey(m_Window.m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(m_Window.m_Window, true);

			// ------
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// render
			m_BlueShader->Bind();
			m_SquareVA->Bind();

			glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			//render
            m_Shader->Bind();
			m_VertexArray->Bind();// seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
            glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

            // glBindVertexArray(0); // no need to unbind it every time 
			m_Window.OnUpdate();
		}
	}
}