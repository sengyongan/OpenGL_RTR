#include "Renderer.h"

#include"Shader.h"
#include"Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//temp
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Opengl {

	// settings
	float vertices[3 * 7] = {//position + color
		-0.5f, -0.5f, 0.0f,		0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f,		0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f,		0.8f, 0.8f, 0.2f, 1.0f
	};
	float squareVertices[] = {//position + texture左下，右下，右上，左上
		-0.75f, -0.75f, 0.0f,	0.0f, 0.0f,
		 0.75f, -0.75f, 0.0f,	1.0f, 0.0f,
		 0.75f,  0.75f, 0.0f,	1.0f, 1.0f,
		-0.75f,  0.75f, 0.0f,	0.0f, 1.0f
	};
	float cube_Vertices[] = {//后，前，左，右，下，上（左下，右下，右上，右上，左上，左下）
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	//顶点索引
	unsigned int indices[3] = { 0, 1, 2 };

	unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

	unsigned int cube_Indices[36] = { 
		0, 1, 2, 2, 3, 0 ,
		4,5,6,6,7,4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20
	};

	struct RendererData {

		std::shared_ptr<VertexArray> QuadVertexArray;
		std::shared_ptr<VertexBuffer> QuadVertexBuffer;
		std::shared_ptr<Shader> QuadShader;
		std::shared_ptr<Texture> QuadTexture1;
		std::shared_ptr<Texture> QuadTexture2;

		std::shared_ptr<VertexArray> TrianglesVertexArray;
		std::shared_ptr<VertexBuffer> TrianglesVertexBuffer;
		std::shared_ptr<Shader> TrianglesShader;


	};
	static RendererData s_Data;

	void Renderer::BeginScene()
	{
	}
	void Renderer::init()
	{
		s_Data.QuadShader.reset(new Shader("D:\\OpenGL_C++_Demo\\OpenGl_Demo\\OpenGl\\src\\shader\\blue_Vertex_Shader.glsl", "D:\\OpenGL_C++_Demo\\OpenGl_Demo\\OpenGl\\src\\shader\\blue_Fragment_Shader.glsl"));
		s_Data.TrianglesShader.reset(new Shader("D:\\OpenGL_C++_Demo\\OpenGl_Demo\\OpenGl\\src\\shader\\vs.vs", "D:\\OpenGL_C++_Demo\\OpenGl_Demo\\OpenGl\\src\\shader\\fs.fs"));//设置智能指针指向的对象
		//
		s_Data.QuadTexture1 = std::make_unique<Texture>("D:\\OpenGL_C++_Demo\\OpenGl_Demo\\OpenGl\\src\\assest\\textures\\container.jpg");
		s_Data.QuadTexture2 = std::make_unique<Texture>("D:\\OpenGL_C++_Demo\\OpenGl_Demo\\OpenGl\\src\\assest\\textures\\awesomeface.png");

		///////////////////////////////////////////////////////////////////////
		/////Quad///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		s_Data.QuadVertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(cube_Vertices, sizeof(cube_Vertices)));

		squareVB->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float2, "a_TexCoord" }
			}
		);

		s_Data.QuadVertexArray->AddVertexBuffer(squareVB);

		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(cube_Indices, sizeof(cube_Indices) / sizeof(unsigned int)));

		s_Data.QuadVertexArray->SetIndexBuffer(squareIB);
		///////////////////////////////////////////////////////////////////////
		/////Triangles///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		s_Data.TrianglesVertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> vertexBuffer;//因为全局变量只需要VAO即可
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
					{ ShaderDataType::Float3, "a_Position" },
					{ ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		//进行布局
		s_Data.TrianglesVertexArray->AddVertexBuffer(vertexBuffer);

		//m_IndexBuffer = std::make_unique<OpenGLIndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));//获取个数
		
		s_Data.TrianglesVertexArray->SetIndexBuffer(indexBuffer);
		///////////////////////////////////////////////////////////////////////////
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetInt("texture1", 0);
		s_Data.QuadShader->SetInt("texture2", 1);
	}
	void Renderer::EndScene()
	{		
		// render方形
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;//sin值变为（-1——1），/2+0.5-》0——1
		glm::vec4 result = glm::vec4(0.0f, greenValue, 0.0f, 1.0f);
		//
		glActiveTexture(GL_TEXTURE0);
		s_Data.QuadTexture1->Bind();
		glActiveTexture(GL_TEXTURE1);
		s_Data.QuadTexture2->Bind();
		//
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);

		//
		s_Data.QuadShader->Bind();

		//s_Data.QuadShader->SetMat4("model", model);
		s_Data.QuadShader->SetMat4("view", view);
		s_Data.QuadShader->SetMat4("projection", projection);

		s_Data.QuadShader->SetFloat4("ourColor", result);

		s_Data.QuadVertexArray->Bind();
		//
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model5;
			model5 = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i+1;
			model5 = glm::rotate(model5, glm::radians(40.0f) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
			s_Data.QuadShader->SetMat4("model", model5);

			Renderer::DrawIndexed(s_Data.QuadVertexArray);
			//glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//

		//Renderer::DrawIndexed(s_Data.QuadVertexArray);

		s_Data.QuadShader->Unbind();

		//render////////////////////

		//三角形
		// create transformations
		glm::mat4 model1 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 view1 = glm::mat4(1.0f);

		model = glm::rotate(model1, glm::radians(-70.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::translate(model1, glm::vec3(0.0f, 0.0f, -5.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);


		s_Data.TrianglesShader->Bind();

		s_Data.TrianglesShader->SetMat4("model", model);
		s_Data.TrianglesShader->SetMat4("view", view);
		s_Data.TrianglesShader->SetMat4("projection", projection);

		s_Data.TrianglesVertexArray->Bind();// seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		Renderer::DrawIndexed(s_Data.TrianglesVertexArray);
		
		s_Data.TrianglesShader->Unbind();

		//
		glm::mat4 model2 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 view2 = glm::mat4(1.0f);

		model = glm::rotate(model2, glm::radians(50.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::translate(view2, glm::vec3(0.0f, 0.0f, -5.0f));


		s_Data.TrianglesShader->Bind();

		s_Data.TrianglesShader->SetMat4("model", model);
		s_Data.TrianglesShader->SetMat4("view", view);
		s_Data.TrianglesShader->SetMat4("projection", projection);

		s_Data.TrianglesVertexArray->Bind();// seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		Renderer::DrawIndexed(s_Data.TrianglesVertexArray);


	}
	void Renderer::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void Renderer::Clear()
	{
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void Renderer::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}