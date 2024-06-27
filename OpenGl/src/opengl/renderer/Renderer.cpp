#include "Renderer.h"

#include"Shader.h"
#include"Texture.h"
#include"opengl/draw/Model.h"
#include"opengl/core/App.h"
#include"Framebuffer.h"
#include"Uniform.h"
//
#include"opengl/draw/DrawCube.h"
#include"opengl/draw/DrawPointLight.h"
#include"opengl/draw/DrawPoint.h"
#include"opengl/draw/DrawQuad.h"
#include"opengl/draw/DrawTriangles.h"
#include"opengl/draw/DrawScreenQuad.h"
#include"opengl/draw/DrawSkybox.h"
#include"opengl/draw/DrawGeometry.h"
#include"opengl/draw/DrawPlanet.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//temp
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Opengl {

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
	vector<glm::vec3> windows
	{
		glm::vec3(-2.5f,  -3.5f, -2.0f),
		glm::vec3(-2.3f,  -3.5f,  1.0f),
		glm::vec3(-2.0f,  -3.5f,  0.9f),
		glm::vec3(-2.3f,  -3.5f, -0.0f),
		glm::vec3(-2.0f,  -3.5f, -0.6f)
	};

	//RendererData
	struct RendererData {
		//Draw/////////////////////////////////////////////////////
		std::shared_ptr<DrawCube> m_DrawCube;
		std::shared_ptr<DrawPointLight> m_DrawPointLight;
		std::shared_ptr<DrawPoint> m_DrawPoint;
		std::shared_ptr<DrawQuad> m_DrawQuad;
		std::shared_ptr<DrawTriangles> m_DrawTriangles;

		std::shared_ptr<DrawPointLight> m_DrawStencil;

		std::shared_ptr<DrawScreenQuad> m_DrawScreenQuad;
		//Texture//////////////////////////////////////////////////
		std::shared_ptr<Texture> Texture1;
		std::shared_ptr<Texture> Texture2;
		std::shared_ptr<Texture> Texture3;
		std::shared_ptr<Texture> grass_Texture;
		std::shared_ptr<Texture> window_Texture;
		std::shared_ptr<Texture> metal_Texture;
		//skybox///////////////////////////////////////////////////
		vector<std::string> CubeTexturePath{
			"../OpenGl/resources/skybox/right.jpg",
			"../OpenGl/resources/skybox/left.jpg",
			"../OpenGl/resources/skybox/top.jpg",
			"../OpenGl/resources/skybox/bottom.jpg",
			"../OpenGl/resources/skybox/back.jpg",
			"../OpenGl/resources/skybox/front.jpg"
		};
		std::shared_ptr<DrawSkybox> m_DrawSkybox;
		std::shared_ptr<Texture> cube_Texture;
		std::shared_ptr<Shader> SkyboxShader;
		//geometry///////////////////////////////////////////////////
		std::shared_ptr<DrawGeometry> m_DrawGeometry;
		std::shared_ptr<Shader> GeometryShader;
		std::shared_ptr<Shader> ModelNormalShader;
		//Planet && rock///////////////////////////////////////////////////
		std::shared_ptr<DrawPlanet> m_DrawPlanet;
		std::shared_ptr<Shader> PlanetShader;
		std::shared_ptr<Shader> RockShader;
		
		//FrameBuffer//////////////////////////////////////////////
		std::shared_ptr<Framebuffer> Multisample_FrameBuffer;
		std::shared_ptr<Texture> Screen_Texture;
		//Shader///////////////////////////////////////////////////
		//quad
		std::shared_ptr<Shader> QuadShader;
		//Triangles
		std::shared_ptr<Shader> TrianglesShader;
		//cube
		std::shared_ptr<Shader> CubeShader;
		//whilte_Cube
		std::shared_ptr<Shader> PointLightShader;
		std::vector<glm::vec3> lightColors;/////////////
		//point
		std::shared_ptr<Shader> PointShader;
		//modle
		std::shared_ptr<Shader> ModelShader;
		std::shared_ptr<Model> m_Model;
		//
		std::shared_ptr<Shader> SceneShader;
		//UniformBuffer////////////////////////////////////////////
		std::shared_ptr<Uniform> uniformBuffer;
	};
	static RendererData s_Data;


	void Renderer::BeginScene(const EditorCamera& camera)
	{
		//uniformData_BindPoint
		glm::mat4 ViewProjection = camera.GetViewProjection();
		s_Data.uniformBuffer->SetData(glm::value_ptr(ViewProjection), sizeof(glm::mat4),0);

	}

	void Renderer::init()
	{
		//
		s_Data.QuadShader.reset(new Shader("../OpenGl/src/shader/quad_Vertex_Shader.glsl", "../OpenGl/src/shader/quad_Fragment_Shader.glsl"));//设置智能指针指向的对象
		s_Data.TrianglesShader.reset(new Shader("../OpenGl/src/shader/triangles_Vertex_Shader.glsl", "../OpenGl/src/shader/triangles_Fragment_Shader.glsl"));//设置智能指针指向的对象
		s_Data.CubeShader.reset(new Shader("../OpenGl/src/shader/multiple_lights.vs_glsl", "../OpenGl/src/shader/multiple_lights.fs_glsl"));
		s_Data.PointLightShader.reset(new Shader("../OpenGl/src/shader/pointLight_Vertex_Shader.glsl", "../OpenGl/src/shader/pointLight_Fragment_Shader.glsl"));
		s_Data.PointShader.reset(new Shader("../OpenGl/src/shader/point_Vertex_Shader.glsl", "../OpenGl/src/shader/point_Fragment_Shader.glsl"));
		s_Data.ModelShader.reset(new Shader("../OpenGl/src/shader/model_Vertex_Shader.glsl", "../OpenGl/src/shader/model_Fragment_Shader.glsl"));
		s_Data.ModelNormalShader.reset(new Shader("../OpenGl/src/shader/modelNormal_Vertex_Shader.glsl", "../OpenGl/src/shader/modelNormal_Fragment_Shader.glsl"
		  ,"../OpenGl/src/shader/modelNormal_geometry_Shader.glsl"));
		s_Data.SceneShader.reset(new Shader("../OpenGl/src/shader/screen_Vertex_Shader.glsl", "../OpenGl/src/shader/screen_Fragment_Shader.glsl"));
		s_Data.SkyboxShader.reset(new Shader("../OpenGl/src/shader/skybox_Vertex_Shader.glsl", "../OpenGl/src/shader/skybox_Fragment_Shader.glsl"));
		s_Data.GeometryShader.reset(new Shader("../OpenGl/src/shader/geometry_Vertex_Shader.glsl", "../OpenGl/src/shader/geometry_Fragment_Shader.glsl",
			"../OpenGl/src/shader/geometry_geometry_Shader.glsl"));
		s_Data.PlanetShader.reset(new Shader("../OpenGl/src/shader/model.vs", "../OpenGl/src/shader/model.fs"));
		s_Data.RockShader.reset(new Shader("../OpenGl/src/shader/Instance.vs", "../OpenGl/src/shader/Instance.fs"));
		//
		s_Data.Texture1 = std::make_unique<Texture>("../OpenGl/resources/textures/container2.png");
		s_Data.Texture2 = std::make_unique<Texture>("../OpenGl/resources/textures/ChernoLogo.png");
		s_Data.Texture3 = std::make_unique<Texture>("../OpenGl/resources/textures/container2_specular.png");
		s_Data.grass_Texture = std::make_unique<Texture>("../OpenGl/resources/textures/grass.png");
		s_Data.window_Texture = std::make_unique<Texture>("../OpenGl/resources/textures/window.png");
		s_Data.metal_Texture = std::make_unique<Texture>("../OpenGl/resources/textures/metal.png");
		s_Data.cube_Texture = std::make_unique<Texture>();
		s_Data.cube_Texture->loadCubemap(s_Data.CubeTexturePath);
		//
		//
		FramebufferSpecification fbSpec;
		fbSpec.Width = 1600;
		fbSpec.Height = 900;
		s_Data.Multisample_FrameBuffer = std::make_unique<Framebuffer>(fbSpec);
		s_Data.Multisample_FrameBuffer->Unbind();
		//s_Data.FrameBuffer = std::make_unique<Framebuffer>(fbSpec);
		//s_Data.FrameBuffer->initColorAttachment();
		//s_Data.FrameBuffer->Unbind();
		//
		//s_Data.m_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/nanosuit/nanosuit.obj");
		//s_Data.m_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/cyborg/cyborg.obj");
		s_Data.m_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/backpack/backpack.obj");
		//s_Data.m_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/planet/planet.obj");
		//s_Data.m_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/vampire/vampire.obj");
		//init
		s_Data.m_DrawCube = std::make_unique<DrawCube>();
		s_Data.m_DrawCube->Bind();
		s_Data.m_DrawPointLight = std::make_unique<DrawPointLight>();
		s_Data.m_DrawPointLight->Bind();
		s_Data.m_DrawPoint = std::make_unique<DrawPoint>();
		s_Data.m_DrawPoint->Bind();
		s_Data.m_DrawQuad = std::make_unique<DrawQuad>();
		s_Data.m_DrawQuad->Bind();
		s_Data.m_DrawTriangles = std::make_unique<DrawTriangles>();
		s_Data.m_DrawTriangles->Bind();

		s_Data.m_DrawStencil = std::make_unique<DrawPointLight>();
		s_Data.m_DrawStencil->Bind();

		s_Data.m_DrawScreenQuad = std::make_unique<DrawScreenQuad>();
		s_Data.m_DrawScreenQuad->Bind();

		s_Data.m_DrawSkybox = std::make_unique<DrawSkybox>();
		s_Data.m_DrawSkybox->Bind();

		s_Data.m_DrawGeometry = std::make_unique<DrawGeometry>();
		s_Data.m_DrawGeometry->Bind();

		s_Data.m_DrawPlanet = std::make_unique<DrawPlanet>();
		s_Data.m_DrawPlanet->Bind();

		//uniformBuffer//////////////////////////////////////////////////////////
		//uniformBuffer_BindPoint
		unsigned int uniformBlockIndex_QuadShader = glGetUniformBlockIndex(s_Data.QuadShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.QuadShader->GetShaderProgram(), uniformBlockIndex_QuadShader, 0);
		unsigned int uniformBlockIndex_TrianglesShader = glGetUniformBlockIndex(s_Data.TrianglesShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.TrianglesShader->GetShaderProgram(), uniformBlockIndex_TrianglesShader, 0);
		unsigned int uniformBlockIndex_CubeShader = glGetUniformBlockIndex(s_Data.CubeShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.CubeShader->GetShaderProgram(), uniformBlockIndex_CubeShader, 0);
		unsigned int uniformBlockIndex_PointLightShader = glGetUniformBlockIndex(s_Data.PointLightShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.PointLightShader->GetShaderProgram(), uniformBlockIndex_PointLightShader, 0);
		unsigned int uniformBlockIndex_PointShader = glGetUniformBlockIndex(s_Data.PointShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.PointShader->GetShaderProgram(), uniformBlockIndex_PointShader, 0);
		unsigned int uniformBlockIndex_ModelShader = glGetUniformBlockIndex(s_Data.ModelShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.ModelShader->GetShaderProgram(), uniformBlockIndex_ModelShader, 0);
		unsigned int uniformBlockIndex_ModelNormalShader = glGetUniformBlockIndex(s_Data.ModelNormalShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.ModelNormalShader->GetShaderProgram(), uniformBlockIndex_ModelNormalShader, 0);
		unsigned int uniformBlockIndex_GeometryShader = glGetUniformBlockIndex(s_Data.GeometryShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.GeometryShader->GetShaderProgram(), uniformBlockIndex_GeometryShader, 0);
		
		unsigned int uniformBlockIndex_PlanetShader = glGetUniformBlockIndex(s_Data.PlanetShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.PlanetShader->GetShaderProgram(), uniformBlockIndex_PlanetShader, 0);
		unsigned int uniformBlockIndex_RockShader = glGetUniformBlockIndex(s_Data.RockShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.RockShader->GetShaderProgram(), uniformBlockIndex_RockShader, 0);
		//uniformBuffer_GenBuffer
		s_Data.uniformBuffer = std::make_unique<Uniform>(sizeof(glm::mat4) , 0);


		/////////////////////////////////////////////////////////////////////
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

		//texture->setInt///////////////////////////////////////////////////////////////////
		s_Data.CubeShader->Bind();
		s_Data.CubeShader->SetInt("material.texture1", 0);
		s_Data.CubeShader->SetInt("material.texture2", 1);
		s_Data.CubeShader->SetInt("material.specular_Texture", 2);
		s_Data.CubeShader->SetInt("material.texture_back", 3);
		//
		s_Data.QuadShader->Bind();

		s_Data.QuadShader->SetInt("texture1", 0);
		//
		s_Data.SceneShader->Bind();

		s_Data.SceneShader->SetInt("screenTexture", 0);
		//
		s_Data.SkyboxShader->Bind();

		s_Data.SkyboxShader->SetInt("skybox", 0);
		//
		//s_Data.ModelShader->Bind();

		//s_Data.ModelShader->SetInt("skybox", 2);
	}
	void Renderer::EndScene()
	{		
		//Multisample_FrameBuffer		
		s_Data.Multisample_FrameBuffer->framebuffer_size();

		s_Data.Multisample_FrameBuffer->Unbind();
		s_Data.Multisample_FrameBuffer->BindMultisample();
		//init
		Renderer::Clear();//需要放在这个位置，清除自己的帧缓冲
		//
		glm::mat4 model = glm::mat4(1.0f);
		//GreenValue
		float timeValue = glfwGetTime();
		float GreenValue = (sin(timeValue) / 2.0f) + 0.5f;//sin值变为（-1——1），/2+0.5-》0——1
		glm::vec4 result = glm::vec4(0.0f, GreenValue, 0.0f, 1.0f);
		//三角形///////////////////////////////////////////////////////////////////////////
		//三角形///////////////////////////////////////////////////////////////////////////
		//三角形///////////////////////////////////////////////////////////////////////////
		//s_Data.TrianglesShader->Bind();
		//model = glm::scale(glm::mat4(1.0f), glm::vec3(30.0f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::translate(model, glm::vec3 (0.0f, 0.0f, -1.0f));
		//s_Data.TrianglesShader->SetMat4("model", model);

		//s_Data.m_DrawTriangles->OnDraw(s_Data.TrianglesShader);
		//geometry///////////////////////////////////////////////////////////////////////////
		//geometry///////////////////////////////////////////////////////////////////////////
		//geometry///////////////////////////////////////////////////////////////////////////
		s_Data.GeometryShader->Bind();
		model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
		s_Data.GeometryShader->SetMat4("model", model);

		s_Data.m_DrawGeometry->OnDraw(s_Data.GeometryShader);
		//instance///////////////////////////////////////////////////////////////////////////
		//instance///////////////////////////////////////////////////////////////////////////
		//instance///////////////////////////////////////////////////////////////////////////
		//s_Data.m_DrawPlanet->OnDrawPlanet(s_Data.PlanetShader);
		///Planet
		glActiveTexture(GL_TEXTURE2);
		s_Data.cube_Texture->BindCubeTexture();

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
		s_Data.ModelShader->SetFloat3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		s_Data.ModelShader->SetFloat3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		// point light 1
		for (int i = 0; i < 10; i++) {
			s_Data.ModelShader->SetFloat3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
			s_Data.ModelShader->SetFloat3("pointLights[" + std::to_string(i) + "].color", s_Data.lightColors[i]);

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

		s_Data.m_DrawPlanet->OnDrawPlanet(s_Data.ModelShader);

		///Rock
		glActiveTexture(GL_TEXTURE2);
		s_Data.cube_Texture->BindCubeTexture();

		s_Data.RockShader->Bind();

		//
		s_Data.RockShader->SetFloat("shininess", 32.0f);
		//
		s_Data.RockShader->SetFloat3("constVal.camera_Position", App::Get().GetCamera().GetPosition());
		s_Data.RockShader->SetFloat3("constVal.camera_Direction", App::Get().GetCamera().GetForwardDirection());
		s_Data.RockShader->SetFloat("constVal.constant", 1.0f);
		s_Data.RockShader->SetFloat("constVal.linear", 0.09f);
		s_Data.RockShader->SetFloat("constVal.quadratic", 0.032f);
		//

		// directional light
		s_Data.RockShader->SetFloat3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		s_Data.RockShader->SetFloat3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		s_Data.RockShader->SetFloat3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		s_Data.RockShader->SetFloat3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		// point light 1
		for (int i = 0; i < 10; i++) {
			s_Data.RockShader->SetFloat3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
			s_Data.RockShader->SetFloat3("pointLights[" + std::to_string(i) + "].color", s_Data.lightColors[i]);

			s_Data.RockShader->SetFloat3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			s_Data.RockShader->SetFloat3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			s_Data.RockShader->SetFloat3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		}

		// spotLight

		s_Data.RockShader->SetFloat3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		s_Data.RockShader->SetFloat3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		s_Data.RockShader->SetFloat3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		s_Data.RockShader->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		s_Data.RockShader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		//s_Data.m_DrawPlanet->OnDrawRock(s_Data.RockShader);
		s_Data.m_DrawPlanet->OnDrawRock(s_Data.RockShader);
		//skybox///////////////////////////////////////////////////////////////////////////
		//skybox///////////////////////////////////////////////////////////////////////////
		//skybox///////////////////////////////////////////////////////////////////////////
		glActiveTexture(GL_TEXTURE0);
		s_Data.cube_Texture->BindCubeTexture();
		glDepthFunc(GL_LEQUAL);
		s_Data.SkyboxShader->Bind();
		
		glm::mat4 view = glm::mat4(glm::mat3(App::Get().GetCamera().GetViewMatrix()));
		glm::mat4 projection = App::Get().GetCamera().GetProjection();
		s_Data.SkyboxShader->SetMat4("view", view);
		s_Data.SkyboxShader->SetMat4("projection", projection);

		s_Data.m_DrawSkybox->OnDraw(s_Data.SkyboxShader);
		s_Data.SkyboxShader->Bind();
		glDepthFunc(GL_LESS);

		//grass///////////////////////////////////////////////////////////////////////////
		//grass///////////////////////////////////////////////////////////////////////////
		//grass///////////////////////////////////////////////////////////////////////////
		glActiveTexture(GL_TEXTURE0);
		s_Data.grass_Texture->Bind();

		s_Data.QuadShader->Bind();
		model = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
		model = glm::translate(model, glm::vec3 (-5.0f, 0.0f, 0.0f));
		s_Data.QuadShader->SetFloat4("ourColor", glm::vec4(1.0f,1.0f,1.0f,1.0f));
		s_Data.QuadShader->SetMat4("model", model);

		s_Data.m_DrawQuad->OnDraw(s_Data.QuadShader);
		//plane地面//////////////////////////////////////////////////////////////////////////////////
		//plane地面//////////////////////////////////////////////////////////////////////////////////
		//plane地面//////////////////////////////////////////////////////////////////////////////////
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		glActiveTexture(GL_TEXTURE0);
		s_Data.metal_Texture->Bind();

		s_Data.CubeShader->Bind();

		s_Data.CubeShader->SetFloat("material.shininess", 32.0f);
		//
		s_Data.CubeShader->SetFloat3("constVal.camera_Position", App::Get().GetCamera().GetPosition());
		s_Data.CubeShader->SetFloat3("constVal.camera_Direction", App::Get().GetCamera().GetForwardDirection());
		s_Data.CubeShader->SetFloat("constVal.constant", 1.0f);
		s_Data.CubeShader->SetFloat("constVal.linear", 0.09f);
		s_Data.CubeShader->SetFloat("constVal.quadratic", 0.032f);
		s_Data.CubeShader->SetFloat("constVal.blinn", true);
		//

		// directional light
		s_Data.CubeShader->SetFloat3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		s_Data.CubeShader->SetFloat3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		s_Data.CubeShader->SetFloat3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		s_Data.CubeShader->SetFloat3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		// point light 1
		for (int i = 0; i < 10; i++) {
			s_Data.CubeShader->SetFloat3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
			s_Data.CubeShader->SetFloat3("pointLights[" + std::to_string(i) + "].color", s_Data.lightColors[i]);

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
		model = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 1.0f, 50.0f));
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, -0.1f));
		s_Data.CubeShader->SetMat4("model", model);
		s_Data.m_DrawCube->OnDraw(s_Data.CubeShader);
		//
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		float scaleVal = 1.01f;
		s_Data.PointLightShader->Bind();
		model = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 1.0f, 50.0f) * scaleVal);
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, -0.1f));
		s_Data.PointLightShader->SetMat4("model", model);
		s_Data.PointLightShader->SetFloat3("color", s_Data.lightColors[0]);
		s_Data.m_DrawStencil->OnDraw(s_Data.PointLightShader);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);

		// cube//////////////////////////////////////////////////////////////////////////////
		// cube//////////////////////////////////////////////////////////////////////////////
		// cube//////////////////////////////////////////////////////////////////////////////
		//
		glActiveTexture(GL_TEXTURE0);
		s_Data.Texture1->Bind();
		glActiveTexture(GL_TEXTURE1);
		s_Data.Texture2->Bind();
		glActiveTexture(GL_TEXTURE2);
		s_Data.Texture3->Bind();
		glActiveTexture(GL_TEXTURE3);
		s_Data.Texture3->Bind();
		//
		s_Data.CubeShader->Bind();
		//
		//
		
		//s_Data.CubeVertexArray->Bind();
		//
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			float angle = 20.0f * i+1;
			model = glm::rotate(model, glm::radians(40.0f) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
			s_Data.CubeShader->SetMat4("model", model);

			//Renderer::DrawIndexed(s_Data.CubeVertexArray);
			s_Data.m_DrawCube->OnDraw(s_Data.CubeShader);
		}
		// pointLight//////////////////////////////////////////////////////////////////////////////
		// pointLight//////////////////////////////////////////////////////////////////////////////
		// pointLight//////////////////////////////////////////////////////////////////////////////
		s_Data.PointLightShader->Bind();
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); 
			s_Data.PointLightShader->SetMat4("model", model);
			s_Data.PointLightShader->SetFloat3("color",s_Data.lightColors[i]);
			s_Data.m_DrawPointLight->OnDraw(s_Data.PointLightShader);

		}
			//model = glm::mat4(1.0f);
			//model = glm::translate(model, glm::vec3(1.0f, 1.0f, 1.0f));
			//s_Data.PointLightShader->SetFloat3("color", glm::vec3(1.0f, 1.0f, 1.0f));
			//s_Data.m_DrawPointLight->OnDraw(s_Data.PointLightShader);
		// point//////////////////////////////////////////////////////////////////////////////
		// point//////////////////////////////////////////////////////////////////////////////
		// point//////////////////////////////////////////////////////////////////////////////
		s_Data.PointShader->Bind();
		model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 5.0f,10.0f));
		s_Data.PointShader->SetMat4("model", model);
		s_Data.PointShader->SetFloat3("color", result);
		s_Data.m_DrawPoint->OnDraw(s_Data.PointShader);

		//modle//////////////////////////////////////////////////////////////////////////////////
		//modle//////////////////////////////////////////////////////////////////////////////////
		//modle//////////////////////////////////////////////////////////////////////////////////
		//glActiveTexture(GL_TEXTURE2);
		//s_Data.cube_Texture->BindCubeTexture();

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
		s_Data.ModelShader->SetFloat3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		s_Data.ModelShader->SetFloat3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		// point light 1
		for (int i = 0; i < 10; i++) {
			s_Data.ModelShader->SetFloat3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
			s_Data.ModelShader->SetFloat3("pointLights[" + std::to_string(i) + "].color", s_Data.lightColors[i]);

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
		model = glm::scale(glm::mat4(1.0f), glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::translate(model, glm::vec3(5.0f, 5.0f, 0.0f)); 
		s_Data.ModelShader->SetMat4("model", model);

		s_Data.m_Model->Draw(s_Data.ModelShader);

		//normal
		//s_Data.ModelNormalShader->Bind();
		//s_Data.ModelNormalShader->SetMat4("model", model);

		//s_Data.m_Model->Draw(s_Data.ModelNormalShader);

		//window///////////////////////////////////////////////////////////////////////////
		//window///////////////////////////////////////////////////////////////////////////
		//window///////////////////////////////////////////////////////////////////////////
		glActiveTexture(GL_TEXTURE0);
		s_Data.window_Texture->Bind();
		s_Data.QuadShader->Bind();
		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < windows.size(); i++)
		{
			float distance = glm::length(App::Get().GetCamera().GetPosition() - windows[i]);
			sorted[distance] = windows[i];
		}
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, it->second);
			s_Data.QuadShader->SetMat4("model", model);
			s_Data.QuadShader->SetFloat4("ourColor", glm::vec4(0.3f, 1.0f, 1.0f, 1.0f));
			s_Data.m_DrawQuad->OnDraw(s_Data.QuadShader);
		}
		s_Data.QuadShader->Unbind();
		//framebuffer///////////////////////////////////////////////////////////////////////////
		//framebuffer///////////////////////////////////////////////////////////////////////////
		//framebuffer///////////////////////////////////////////////////////////////////////////

		s_Data.Multisample_FrameBuffer->BlitFramebuffer();
		s_Data.Multisample_FrameBuffer->Unbind();

		glDisable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);
		//DrawQuad
		s_Data.SceneShader->Bind();
		s_Data.m_DrawScreenQuad->Bind();
		s_Data.SceneShader->SetInt("screenWidth_mid", App::Get().GetWindow().GetNewWidth());
		s_Data.SceneShader->SetInt("screenHeight_mid", App::Get().GetWindow().GetNewHeight());

		glActiveTexture(GL_TEXTURE0);
		s_Data.Multisample_FrameBuffer->BindTexture();

		s_Data.m_DrawScreenQuad->OnDraw(s_Data.SceneShader);

	}
	void Renderer::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void Renderer::Clear()
	{

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_PROGRAM_POINT_SIZE);

		//glEnable(GL_MULTISAMPLE);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);



	}
	void Renderer::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::DrawPoints(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_POINTS, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
	void Renderer::DrawLines(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_LINE_STRIP, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}


}