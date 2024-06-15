#include "Renderer.h"

#include"Shader.h"
#include"Texture.h"

#include"App.h"
#include"Model.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//temp
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Opengl {

	// Triangles
	float vertices[3 * 7] = {//position + color
		-0.5f, -0.5f, 0.0f,		0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f,		0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f,		0.8f, 0.8f, 0.2f, 1.0f
	};
	//quad
	float squareVertices[] = {//position + texture左下，右下，右上，左上
		-0.75f, -0.75f, 0.0f,	0.0f, 0.0f,
		 0.75f, -0.75f, 0.0f,	1.0f, 0.0f,
		 0.75f,  0.75f, 0.0f,	1.0f, 1.0f,
		-0.75f,  0.75f, 0.0f,	0.0f, 1.0f
	};
	//cube(postion)
	float point_Vertices[] = {//后，前，左，右，下，上（左下，右下，右上，右上，左上，左下）
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f
	};
	//cube(postion, TexCoord, normal)
	float cube_Vertices[] = {//后，前，左，右，下，上（左下，右下，右上，右上，左上，左下）
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f
	};
	//multiple * 10 positions
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
	//
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -5.0f),
		glm::vec3(1.7f,  0.2f,  2.3f),
		glm::vec3(2.5f, -3.3f, -6.0f),
		glm::vec3(4.0f,  2.0f, -3.0f),
		glm::vec3(0.7f,  1.0f,  2.0f),
		glm::vec3(2.1f, -3.0f, -1.0f),
		glm::vec3(-4.0f,  2.0f, -1.0f),
		glm::vec3(0.7f,  0.2f,  1.0f),
		glm::vec3(1.0f, 3.1f, -4.0f),
		glm::vec3(2.0f,  2.0f, 5.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	//顶点索引
	//Triangles
	unsigned int triangles_squareIndices[3] = { 0, 1, 2 };
	//quad
	unsigned int square_Indices[6] = { 0, 1, 2, 2, 3, 0 };
	//cube
	unsigned int cube_Indices[36] = { 
		0, 1, 2, 2, 3, 0 ,
		4,5,6,6,7,4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20
	};
	//RendererData
	struct RendererData {
		//Texture
		std::shared_ptr<Texture> Texture1;
		std::shared_ptr<Texture> Texture2;
		std::shared_ptr<Texture> Texture3;
		//quad
		std::shared_ptr<VertexArray> QuadVertexArray;
		std::shared_ptr<VertexBuffer> QuadVertexBuffer;
		std::shared_ptr<Shader> QuadShader;
		//Triangles
		std::shared_ptr<VertexArray> TrianglesVertexArray;
		std::shared_ptr<VertexBuffer> TrianglesVertexBuffer;
		std::shared_ptr<Shader> TrianglesShader;
		//cube
		std::shared_ptr<VertexArray> CubeVertexArray;
		std::shared_ptr<VertexBuffer> CubeVertexBuffer;
		std::shared_ptr<Shader> CubeShader;
		//whilte_Cube
		std::shared_ptr<VertexArray> whilte_CubeVertexArray;
		std::shared_ptr<VertexBuffer> whilte_CubeVertexBuffer;
		std::shared_ptr<Shader> whilte_CubeShader;
		std::vector<glm::vec3> lightColors;
		//modle
		std::shared_ptr<Shader> ModelShader;
		std::shared_ptr<Model> m_Model;
	};
	static RendererData s_Data;

	void Renderer::BeginScene(const EditorCamera& camera)
	{
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_ViewProjection", camera.GetViewProjection());
		s_Data.TrianglesShader->Bind();
		s_Data.TrianglesShader->SetMat4("u_ViewProjection", camera.GetViewProjection());
		s_Data.CubeShader->Bind();
		s_Data.CubeShader->SetMat4("u_ViewProjection", camera.GetViewProjection());
		s_Data.whilte_CubeShader->Bind();
		s_Data.whilte_CubeShader->SetMat4("u_ViewProjection", camera.GetViewProjection());
		s_Data.ModelShader->Bind();
		s_Data.ModelShader->SetMat4("u_ViewProjection", camera.GetViewProjection());
	}

	void Renderer::init()
	{
		s_Data.QuadShader.reset(new Shader("../OpenGl/src/shader/quad_Vertex_Shader.glsl", "../OpenGl/src/shader/quad_Fragment_Shader.glsl"));//设置智能指针指向的对象
		s_Data.TrianglesShader.reset(new Shader("../OpenGl/src/shader/triangles_Vertex_Shader.glsl", "../OpenGl/src/shader/triangles_Fragment_Shader.glsl"));//设置智能指针指向的对象
		s_Data.CubeShader.reset(new Shader("../OpenGl/src/shader/multiple_lights.vs_glsl", "../OpenGl/src/shader/multiple_lights.fs_glsl"));
		s_Data.whilte_CubeShader.reset(new Shader("../OpenGl/src/shader/point_Vertex_Shader.glsl", "../OpenGl/src/shader/point_Fragment_Shader.glsl"));
		s_Data.ModelShader.reset(new Shader("../OpenGl/src/shader/model_Vertex_Shader.glsl", "../OpenGl/src/shader/model_Fragment_Shader.glsl"));
		//
		s_Data.Texture1 = std::make_unique<Texture>("../OpenGl/resources/textures/container2.png");
		s_Data.Texture2 = std::make_unique<Texture>("../OpenGl/resources/textures/ChernoLogo.png");
		s_Data.Texture3 = std::make_unique<Texture>("../OpenGl/resources/textures/container2_specular.png");
		//
		//s_Data.m_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/nanosuit/nanosuit.obj");
		//s_Data.m_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/cyborg/cyborg.obj");
		s_Data.m_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/backpack/backpack.obj");
		//s_Data.m_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/planet/planet.obj");
		//s_Data.m_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/vampire/vampire.obj");

		///////////////////////////////////////////////////////////////////////
		/////quad///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		s_Data.QuadVertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> quad_vertexBuffer;
		quad_vertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		quad_vertexBuffer->SetLayout(
			{
					{ ShaderDataType::Float3, "a_Position" },
					{ ShaderDataType::Float4, "a_Color" }
			}
		);
		//进行布局
		s_Data.QuadVertexArray->AddVertexBuffer(quad_vertexBuffer);

		std::shared_ptr<IndexBuffer> quad_indexBuffer;
		quad_indexBuffer.reset(IndexBuffer::Create(square_Indices, sizeof(square_Indices) / sizeof(uint32_t)));//获取个数
		s_Data.QuadVertexArray->SetIndexBuffer(quad_indexBuffer);

		///////////////////////////////////////////////////////////////////////
		/////Triangles///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		s_Data.TrianglesVertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> vertexBuffer;//因为全局变量只需要VAO即可
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		vertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
			}
		);
		//进行布局
		s_Data.TrianglesVertexArray->AddVertexBuffer(vertexBuffer);

		//m_IndexBuffer = std::make_unique<OpenGLIndexBuffer>(triangles_squareIndices, sizeof(triangles_squareIndices) / sizeof(unsigned int));
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(triangles_squareIndices, sizeof(triangles_squareIndices) / sizeof(unsigned int)));//获取个数
		
		s_Data.TrianglesVertexArray->SetIndexBuffer(indexBuffer);
		///////////////////////////////////////////////////////////////////////
		/////Cube///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		s_Data.CubeVertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(cube_Vertices, sizeof(cube_Vertices)));

		squareVB->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float2, "a_TexCoord" },
				{ ShaderDataType::Float3, "a_Normal" },
			}
		);

		s_Data.CubeVertexArray->AddVertexBuffer(squareVB);

		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(cube_Indices, sizeof(cube_Indices) / sizeof(unsigned int)));

		s_Data.CubeVertexArray->SetIndexBuffer(squareIB);
		///////////////////////////////////////////////////////////////////////
		/////while_Cube///////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		s_Data.whilte_CubeVertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> point_VertexBuffer;
		point_VertexBuffer.reset(VertexBuffer::Create(point_Vertices, sizeof(point_Vertices)));

		point_VertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
			}
		);

		s_Data.whilte_CubeVertexArray->AddVertexBuffer(point_VertexBuffer);

		std::shared_ptr<IndexBuffer> point_IndexBuffer;
		point_IndexBuffer.reset(IndexBuffer::Create(cube_Indices, sizeof(cube_Indices) / sizeof(unsigned int)));

		s_Data.whilte_CubeVertexArray->SetIndexBuffer(point_IndexBuffer);
		///////////////////////////////////////////////////////////////////////////
		srand(300);
		for (unsigned int i = 0; i < 10; i++)
		{
			GLfloat rColor = ((rand() % 100) / 100.0f); // Between 0.5 and 1.0
			GLfloat gColor = ((rand() % 100) / 100.0f); // Between 0.5 and 1.0
			GLfloat bColor = ((rand() % 100) / 100.0f);
			//GLfloat rColor = ((rand() % 100) / 200.0f) + 0.8; // Between 0.5 and 1.0
			//GLfloat gColor = ((rand() % 100) / 200.0f) + 0.8; // Between 0.5 and 1.0
			//GLfloat bColor = ((rand() % 100) / 200.0f) + 0.8;
			s_Data.lightColors.push_back(glm::vec3(rColor, gColor, bColor));
		}

		///////////////////////////////////////////////////////////////////////////
		s_Data.CubeShader->Bind();
		s_Data.CubeShader->SetInt("material.texture1", 0);
		s_Data.CubeShader->SetInt("material.texture2", 1);
		s_Data.CubeShader->SetInt("material.specular_Texture", 2);
	}
	void Renderer::EndScene()
	{		
		//
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		//三角形///////////////////////////////////////////////////////////////////////////
		//三角形///////////////////////////////////////////////////////////////////////////
		//三角形///////////////////////////////////////////////////////////////////////////
		//model = glm::scale(glm::mat4(1.0f), glm::vec3(30.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::translate(model, glm::vec3 (0.0f, 0.0f, -1.0f));

		//s_Data.TrianglesShader->Bind();

		//s_Data.TrianglesShader->SetMat4("model", model);

		//s_Data.TrianglesVertexArray->Bind();// seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//Renderer::DrawIndexed(s_Data.TrianglesVertexArray);
		//
		//s_Data.TrianglesShader->Unbind();
		// cube//////////////////////////////////////////////////////////////////////////////
		// cube//////////////////////////////////////////////////////////////////////////////
		// cube//////////////////////////////////////////////////////////////////////////////
		float timeValue = glfwGetTime();
		float blueValue = (sin(timeValue) / 2.0f) + 0.5f;//sin值变为（-1——1），/2+0.5-》0——1
		glm::vec4 result = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

		//
		glActiveTexture(GL_TEXTURE0);
		s_Data.Texture1->Bind();
		glActiveTexture(GL_TEXTURE1);
		s_Data.Texture2->Bind();
		glActiveTexture(GL_TEXTURE2);
		s_Data.Texture3->Bind();
		//
		s_Data.CubeShader->Bind();
		//
		s_Data.CubeShader->SetFloat("material.shininess", 32.0f);
		//
		s_Data.CubeShader->SetFloat3("constVal.camera_Position", App::Get().GetCamera().GetPosition());
		s_Data.CubeShader->SetFloat3("constVal.camera_Direction", App::Get().GetCamera().GetForwardDirection());
		s_Data.CubeShader->SetFloat("constVal.constant", 1.0f);
		s_Data.CubeShader->SetFloat("constVal.linear", 0.09f);
		s_Data.CubeShader->SetFloat("constVal.quadratic", 0.032f);
		//

		// directional light
		s_Data.CubeShader->SetFloat3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		s_Data.CubeShader->SetFloat3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		s_Data.CubeShader->SetFloat3("dirLight.diffuse", glm::vec3(0.05f, 0.05f, 0.05f));
		s_Data.CubeShader->SetFloat3("dirLight.specular", glm::vec3(0.05f, 0.05f, 0.05f));
		//s_Data.CubeShader->SetFloat3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		//s_Data.CubeShader->SetFloat3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		// point light 1
		for (int i = 0; i < 10; i++) {
			s_Data.CubeShader->SetFloat3("pointLights["+ std::to_string(i) +"].position", pointLightPositions[i]);
			s_Data.CubeShader->SetFloat3("pointLights[" + std::to_string(i) + "].color", s_Data.lightColors[i]);

			//s_Data.CubeShader->SetFloat3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			//s_Data.CubeShader->SetFloat3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
			//s_Data.CubeShader->SetFloat3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(0.2f, 0.2f, 0.2f));
			s_Data.CubeShader->SetFloat3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			s_Data.CubeShader->SetFloat3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			s_Data.CubeShader->SetFloat3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		}

		// spotLight

		s_Data.CubeShader->SetFloat3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		s_Data.CubeShader->SetFloat3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		s_Data.CubeShader->SetFloat3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		s_Data.CubeShader->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		s_Data.CubeShader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		//
		//
		s_Data.CubeVertexArray->Bind();
		//
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			float angle = 20.0f * i+1;
			model = glm::rotate(model, glm::radians(40.0f) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
			s_Data.CubeShader->SetMat4("model", model);

			Renderer::DrawIndexed(s_Data.CubeVertexArray);
		}
		// point//////////////////////////////////////////////////////////////////////////////
		// point//////////////////////////////////////////////////////////////////////////////
		// point//////////////////////////////////////////////////////////////////////////////

		s_Data.whilte_CubeShader->Bind();
		s_Data.whilte_CubeVertexArray->Bind();
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); 
			s_Data.whilte_CubeShader->SetMat4("model", model);
			s_Data.whilte_CubeShader->SetFloat3("color",s_Data.lightColors[i]);
			Renderer::DrawIndexed(s_Data.whilte_CubeVertexArray);
		}

		//plane地面//////////////////////////////////////////////////////////////////////////////////
		//plane地面//////////////////////////////////////////////////////////////////////////////////
		//plane地面//////////////////////////////////////////////////////////////////////////////////
		s_Data.CubeShader->Bind();
		s_Data.CubeVertexArray->Bind();
		model = glm::scale(glm::mat4(1.0f), glm::vec3(20.0f,1.0f,20.0f));
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, -0.1f));
		s_Data.CubeShader->SetMat4("model", model);
		Renderer::DrawIndexed(s_Data.CubeVertexArray);

		s_Data.CubeShader->Unbind();

		//modle//////////////////////////////////////////////////////////////////////////////////
		//modle//////////////////////////////////////////////////////////////////////////////////
		//modle//////////////////////////////////////////////////////////////////////////////////
		s_Data.ModelShader->Bind();

		//
		s_Data.ModelShader->SetFloat("shininess", 32.0f);
		//
		s_Data.ModelShader->SetFloat3("constVal.camera_Position", App::Get().GetCamera().GetPosition());
		s_Data.ModelShader->SetFloat3("constVal.camera_Direction", App::Get().GetCamera().GetForwardDirection());
		s_Data.ModelShader->SetFloat("constVal.constant", 1.0f);
		s_Data.ModelShader->SetFloat("constVal.linear", 0.09f);
		s_Data.ModelShader->SetFloat("constVal.quadratic", 0.032f);
		//

		// directional light
		s_Data.ModelShader->SetFloat3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		s_Data.ModelShader->SetFloat3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		s_Data.ModelShader->SetFloat3("dirLight.diffuse", glm::vec3(0.05f, 0.05f, 0.05f));
		s_Data.ModelShader->SetFloat3("dirLight.specular", glm::vec3(0.05f, 0.05f, 0.05f));
		//s_Data.ModelShader->SetFloat3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		//s_Data.ModelShader->SetFloat3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		// point light 1
		for (int i = 0; i < 10; i++) {
			s_Data.ModelShader->SetFloat3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
			s_Data.ModelShader->SetFloat3("pointLights[" + std::to_string(i) + "].color", s_Data.lightColors[i]);

			//s_Data.ModelShader->SetFloat3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			//s_Data.ModelShader->SetFloat3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
			//s_Data.ModelShader->SetFloat3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(0.2f, 0.2f, 0.2f));
			s_Data.ModelShader->SetFloat3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			s_Data.ModelShader->SetFloat3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			s_Data.ModelShader->SetFloat3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		}

		// spotLight

		s_Data.ModelShader->SetFloat3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		s_Data.ModelShader->SetFloat3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		s_Data.ModelShader->SetFloat3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		s_Data.ModelShader->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		s_Data.ModelShader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));



		//
		model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f)); 
		s_Data.ModelShader->SetMat4("model", model);

		s_Data.m_Model->Draw(*(s_Data.ModelShader));
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