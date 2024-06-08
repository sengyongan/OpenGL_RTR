#include "App.h"

#include"opengl/Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Opengl {
	// settings

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
		

	unsigned int indices[3] = { 0, 1, 2 };

	unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

    App* App::s_Instance = nullptr;

	App::App()
	{
        s_Instance = this;

		//shader
		m_Shader.reset(new Shader("D:\\OpenGL_C++_Demo\\OpenGl_Demo\\OpenGl\\src\\shader\\vs.vs", "D:\\OpenGL_C++_Demo\\OpenGl_Demo\\OpenGl\\src\\shader\\fs.fs"));//设置智能指针指向的对象
		m_BlueShader.reset(new Shader("D:\\OpenGL_C++_Demo\\OpenGl_Demo\\OpenGl\\src\\shader\\blue_Vertex_Shader.glsl", "D:\\OpenGL_C++_Demo\\OpenGl_Demo\\OpenGl\\src\\shader\\blue_Fragment_Shader.glsl"));
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
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));//获取个数
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
		m_Window.~Window();

	}
	void App::Run()
	{
		while(!glfwWindowShouldClose(m_Window.m_Window)){

            // input
			if (glfwGetKey(m_Window.m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(m_Window.m_Window, true);

			// ------
			//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT);

			Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			Renderer::Clear();
			// render
			float timeValue = glfwGetTime();
			float greenValue = (sin(timeValue) / 2.0f) + 0.5f;//sin值变为（-1——1），/2+0.5-》0——1
			glm::vec4 result = glm::vec4(0.0f, greenValue, 0.0f, 1.0f);
			m_BlueShader->Bind();
			m_BlueShader->SetFloat4("ourColor", result);
			//int vertexColorLocation = glGetUniformLocation(m_BlueShader->GetShaderProgram(), "ourColor");
			//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

			m_SquareVA->Bind();

			Renderer::DrawIndexed(m_SquareVA);
			//glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

			//render
            m_Shader->Bind();
			m_VertexArray->Bind();// seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
            //glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
			Renderer::DrawIndexed(m_VertexArray);
            // glBindVertexArray(0); // no need to unbind it every time 
			m_Window.OnUpdate();
		}
	}
}