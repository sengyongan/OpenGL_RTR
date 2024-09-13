#include "Renderer.h"

#include"Shader.h"
#include"Texture.h"
#include"opengl/draw/Model.h"
#include"opengl/core/App.h"
#include"opengl/core/TextRendering.h"
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
#include"opengl/draw/DrawGamma.h"
#include"opengl/draw/DrawTBNQuad.h"
#include"opengl/draw/DrawSphere.h"
#include"opengl/draw/DrawNewCube.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//temp
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <random>

namespace Opengl {
	bool blinn = true;
	//
	bool gammaEnabled = false;
	bool gammaKeyPressed = false;
	//ShadowMap
	glm::mat4 lightProjection, lightView;//光空间矩阵
	glm::mat4 lightSpaceMatrix;//视图投影矩阵

	glm::vec3 lightPos(0.5f, 1.0f, 0.3f);
	float near_plane = 1.0f;
	float far_plane = 25.0f;
	//
	bool shadows = true;
	bool shadowsKeyPressed = false;
	//HDR
	GLboolean hdr = true; // Change with 'Space'
	GLfloat exposure = 0.1f; // Change with Q and E

	float ourLerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}
	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	unsigned int noiseTexture;

	//multiple * 10 positions
	glm::vec3 cubePositions[] = {
		glm::vec3(3.0f,  0.0f,  0.0f),
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
		glm::vec3(-7.5f, -3.5f, -2.0f),
			glm::vec3(-7.3f, -3.5f, 1.0f),
			glm::vec3(-7.0f, -3.5f, 0.9f),
			glm::vec3(-7.3f, -3.5f, -0.0f),
			glm::vec3(-7.0f, -3.5f, -0.6f)
	};
	glm::vec3 lightPositions[] = {
	   glm::vec3(-3.0f, -5.0f, 30.0f),
	   glm::vec3(-1.0f, -5.0f, 30.0f),
	   glm::vec3(1.0f,  -5.0f, 30.0f),
	   glm::vec3(3.0f,  -5.0f, 30.0f)
	};
	glm::vec3 lightColors[] = {
		glm::vec3(5.0f,   5.0f,  5.0f),
		glm::vec3(10.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,   0.0f,  15.0f),
		glm::vec3(0.0f,   5.0f,  0.0f),
		glm::vec3(5.0f,   5.0f,  5.0f),

		glm::vec3(15.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,   0.0f,  10.0f),
		glm::vec3(0.0f,   5.0f,  0.0f),
		glm::vec3(0.0f,   0.0f,  20.0f),
		glm::vec3(0.0f,   10.0f,  0.0f)
	};
	// Light sources
	// - Positions
	vector<glm::vec3> HDR_LightPositions {
		glm::vec3(0.0f, 0.0f, 49.5f),
			glm::vec3(-1.4f, -1.9f, 9.0f),
			glm::vec3(0.0f, -1.8f, 4.0f),
			glm::vec3(0.8f, -1.7f, 6.0f)
	};
	// - Colors
	vector<glm::vec3> HDR_lightColors{
		glm::vec3(200.0f, 200.0f, 200.0f),
			glm::vec3(0.1f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.2f),
			glm::vec3(0.0f, 0.1f, 0.0f)
	};
	vector<glm::vec3> objectPositions{
		glm::vec3(-10.0, -15.0, -10.0),
		glm::vec3(-10.0, -15.0, 0.0),
		glm::vec3(-10.0, -15.0, 10.0),
						  
		glm::vec3(0.0,   -15.0, -10.0),
		glm::vec3(0.0,   -15.0, 0.0),
		glm::vec3(0.0,   -15.0, 10.0),
						  
		glm::vec3(10.0,  -15.0, 0.0),
		glm::vec3(10.0,  -15.0, -10.0),
		glm::vec3(10.0,  -15.0, 10.)
	};
	//PBR
	// ------
	glm::vec3 PBR_lightPositions[] = {
		glm::vec3(-10.0f,  30.0f, 10.0f),
		glm::vec3(10.0f,  30.0f, 10.0f),
		glm::vec3(-10.0f, 20.0f, 10.0f),
		glm::vec3(10.0f, 20.0f, 10.0f),
	};
	glm::vec3 PBR_lightColors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 300.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 300.0f)
	};
	std::vector<glm::vec3> Temp_LightPositions;
	std::vector<glm::vec3> Temp_LightColors;

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
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
		std::shared_ptr<Texture> metalGamma_Texture;
		//PBR//////////////////////////////////////////////////
		std::shared_ptr<Texture> PBR_albedoTexture    ;
		std::shared_ptr<Texture> PBR_aoTexture1 	  ;
		std::shared_ptr<Texture> PBR_metallicTexture2 ;
		std::shared_ptr<Texture> PBR_normallTexture3 	  ;
		std::shared_ptr<Texture> PBR_roughnessTexture4;

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
		std::shared_ptr<Shader> SceneShader;
		//Shadow//////////////////////////////////////////////

		std::shared_ptr<Framebuffer> Shadow_FrameBuffer;
		std::shared_ptr<DrawPoint> m_ShadowDrawPoint;
		std::shared_ptr<Shader> ShadowShader;
		//Point_ShadowMap//////////////////////////////////////////////
		std::shared_ptr<Shader> Point_ShadowMapShader;//万象阴影贴图
		std::shared_ptr<Shader> Point_ShadowShader;//正常渲染 + 阴影绘制

		//Gamma//////////////////////////////////////////////
		std::shared_ptr<Shader> GammaShader;
		std::shared_ptr<DrawGamma> m_DrawGamma;
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
		//HDR///////////////////////////////////////////////////
		std::shared_ptr<Shader> HDRShader;
		std::shared_ptr<Shader> HDRCubeShader;
		//MRT///////////// //////////////////////////////////////
		std::shared_ptr<Shader> MRTShader;
		//UniformBuffer////////////////////////////////////////////
		std::shared_ptr<Shader> PingPongShader;

		//modle///////////////////////////////////////////////////
		std::shared_ptr<Shader> ModelShader;
		std::shared_ptr<Model> m_Model;
		//UniformBuffer////////////////////////////////////////////
		std::shared_ptr<Uniform> uniformBuffer;
		//TBN////////////////////////////////////////////
		std::shared_ptr<Shader> TBNQuadShader;
		std::shared_ptr<DrawTBNQuad> m_DrawTBNQuad;
		std::shared_ptr<Texture> brickWall_Texture;
		std::shared_ptr<Texture> brickWall_Normal_Texture;
		std::shared_ptr<Texture> brickWall_Depth_Texture;
		//G_Buffer && deferred_Light///////////////////////////////////////////////////
		std::shared_ptr<Shader> G_BufferShader;
		std::shared_ptr<Shader> deferred_lightShader;
		std::shared_ptr<Model> m_BackModel;
		//SSAO///////////////////////////////////////////////////
		std::shared_ptr<Shader> G_BufferSSAOShader;
		std::shared_ptr<Shader> G_BufferCubeSSAOShader;
		std::shared_ptr<Shader> SAOShader;
		std::shared_ptr<Shader> SAOBlurShader;
		std::shared_ptr<Shader> SSAOLightShader;
		std::vector<glm::vec3> ssaoKernel;
		std::vector<glm::vec3> ssaoNoise;
		//PBR///////////////////////////////////////////////////
		std::shared_ptr<Shader> PBR_Shader;
		std::shared_ptr<DrawSphere> m_DrawSphere;
		//LBL///////////////////////////////////////////////////
		std::shared_ptr<Texture> LBL_HDRTexture;
		std::shared_ptr<Shader> LBL_RenderCube;
		std::shared_ptr<DrawNewCube> m_DrawNewCube;
		//环境贴图
		std::shared_ptr<Framebuffer> LBL_FrameBuffer;
		std::shared_ptr<Texture> LBL_CubeTexture;
		//背景
		std::shared_ptr<Shader> LBL_Sky;
		//irradiance辐照度
		std::shared_ptr<Shader> LBL_Irradiance;
		std::shared_ptr<Texture> LBL_IrradianceTextureCube;
		//镜面辐照度
		std::shared_ptr<Shader> LBL_IrradianceSpecular;
		std::shared_ptr<Texture> LBL_IrradianceSpecularTextureCube;
		std::shared_ptr<Texture> LBL_BRDFTexture;
		std::shared_ptr<Shader> LBL_BRDF;
		//PBR_Model////////////////////////////////////////////
		std::shared_ptr<Model> Gun_Model;
		std::shared_ptr<Shader> LBL_ModelShader;
		std::shared_ptr<Texture> BackpackAOTexture;
		//Fonts////////////////////////////////////////////
		std::shared_ptr<Shader> FontsShader;
		std::shared_ptr<TextRendering> TextRendering;


	};
	static RendererData s_Data;

	void Renderer::BeginScene(const EditorCamera& camera)
	{
		//uniformData_BindPoint
		glm::mat4 ViewProjection = camera.GetViewProjection();
		s_Data.uniformBuffer->SetData(glm::value_ptr(ViewProjection), sizeof(glm::mat4), 0);

	}

	void Renderer::init()
	{
		//Shader
		s_Data.QuadShader.reset(new Shader("../OpenGl/resources/Shader/shader/quad_Vertex_Shader.glsl", "../OpenGl/resources/Shader/shader/quad_Fragment_Shader.glsl"));//设置智能指针指向的对象
		s_Data.TrianglesShader.reset(new Shader("../OpenGl/resources/Shader/shader/triangles_Vertex_Shader.glsl", "../OpenGl/resources/Shader/shader/triangles_Fragment_Shader.glsl"));//设置智能指针指向的对象
		s_Data.CubeShader.reset(new Shader("../OpenGl/resources/Shader/shader/multiple_lights.vs_glsl", "../OpenGl/resources/Shader/shader/multiple_lights.fs_glsl"));
		s_Data.PointLightShader.reset(new Shader("../OpenGl/resources/Shader/shader/pointLight_Vertex_Shader.glsl", "../OpenGl/resources/Shader/shader/pointLight_Fragment_Shader.glsl"));
		s_Data.PointShader.reset(new Shader("../OpenGl/resources/Shader/shader/point_Vertex_Shader.glsl", "../OpenGl/resources/Shader/shader/point_Fragment_Shader.glsl"));
		s_Data.ModelShader.reset(new Shader("../OpenGl/resources/Shader/shader/model_Vertex_Shader.glsl", "../OpenGl/resources/Shader/shader/model_Fragment_Shader.glsl"));
		s_Data.ModelNormalShader.reset(new Shader("../OpenGl/resources/Shader/shader/modelNormal_Vertex_Shader.glsl", "../OpenGl/resources/Shader/shader/modelNormal_Fragment_Shader.glsl"
			, "../OpenGl/resources/Shader/shader/modelNormal_geometry_Shader.glsl"));
		s_Data.SceneShader.reset(new Shader("../OpenGl/resources/Shader/shader/screen_Vertex_Shader.glsl", "../OpenGl/resources/Shader/shader/screen_Fragment_Shader.glsl"));
		s_Data.SkyboxShader.reset(new Shader("../OpenGl/resources/Shader/shader/skybox_Vertex_Shader.glsl", "../OpenGl/resources/Shader/shader/skybox_Fragment_Shader.glsl"));
		s_Data.GeometryShader.reset(new Shader("../OpenGl/resources/Shader/shader/geometry_Vertex_Shader.glsl", "../OpenGl/resources/Shader/shader/geometry_Fragment_Shader.glsl",
			"../OpenGl/resources/Shader/shader/geometry_geometry_Shader.glsl"));
		s_Data.PlanetShader.reset(new Shader("../OpenGl/resources/Shader/shader/model.vs", "../OpenGl/resources/Shader/shader/model.fs"));
		s_Data.RockShader.reset(new Shader("../OpenGl/resources/Shader/shader/Instance.vs", "../OpenGl/resources/Shader/shader/Instance.fs"));
		s_Data.GammaShader.reset(new Shader("../OpenGl/resources/Shader/newShader/gamma.vs", "../OpenGl/resources/Shader/newShader/gamma.fs"));
		s_Data.ShadowShader.reset(new Shader("../OpenGl/resources/Shader/newShader/ShadowMap.vs", "../OpenGl/resources/Shader/newShader/ShadowMap.fs"));
		s_Data.Point_ShadowMapShader.reset(new Shader("../OpenGl/resources/Shader/newShader/PointShadowMap.vs", "../OpenGl/resources/Shader/newShader/PointShadowMap.fs", "../OpenGl/resources/Shader/newShader/PointShadowMap.gs"));
		s_Data.Point_ShadowShader.reset(new Shader("../OpenGl/resources/Shader/shader/point_Shadow.vs", "../OpenGl/resources/Shader/shader/point_Shadow.fs"));
		s_Data.TBNQuadShader.reset(new Shader("../OpenGl/resources/Shader/newShader/TBNquad.vs", "../OpenGl/resources/Shader/newShader/TBNquad.fs"));
		s_Data.HDRShader.reset(new Shader("../OpenGl/resources/Shader/newShader/HDR.vs", "../OpenGl/resources/Shader/newShader/HDR.fs"));
		s_Data.HDRCubeShader.reset(new Shader("../OpenGl/resources/Shader/newShader/HDRCube.vs", "../OpenGl/resources/Shader/newShader/HDRCube.fs"));
		s_Data.PingPongShader.reset(new Shader("../OpenGl/resources/Shader/newShader/PingPong.vs", "../OpenGl/resources/Shader/newShader/PingPong.fs"));

		s_Data.G_BufferShader.reset(new Shader("../OpenGl/resources/Shader/newShader/g_ModelShader.vs", "../OpenGl/resources/Shader/newShader/g_ModelShader.fs"));
		s_Data.deferred_lightShader.reset(new Shader("../OpenGl/resources/Shader/newShader/deferred.vs", "../OpenGl/resources/Shader/newShader/deferred.fs"));
		//s_Data.MRTShader.reset(new Shader("../OpenGl/resources/Shader/newShader/MRT.vs", "../OpenGl/resources/Shader/newShader/MRT.fs"));
		s_Data.G_BufferSSAOShader.reset(new Shader("../OpenGl/resources/Shader/SSAO/G_Buffer.vs", "../OpenGl/resources/Shader/SSAO/G_Buffer.fs"));
		s_Data.G_BufferCubeSSAOShader.reset(new Shader("../OpenGl/resources/Shader/SSAO/G_CubeBuffer.vs", "../OpenGl/resources/Shader/SSAO/G_Buffer.fs"));
		s_Data.SAOShader.reset(new Shader("../OpenGl/resources/Shader/SSAO/ssao.vs", "../OpenGl/resources/Shader/SSAO/ssao.fs"));
		s_Data.SAOBlurShader.reset(new Shader("../OpenGl/resources/Shader/SSAO/ssao.vs", "../OpenGl/resources/Shader/SSAO/blur.fs"));
		s_Data.SSAOLightShader.reset(new Shader("../OpenGl/resources/Shader/SSAO/ssao.vs", "../OpenGl/resources/Shader/SSAO/light.fs"));
		s_Data.PBR_Shader.reset(new Shader("../OpenGl/resources/Shader/PBR/PBR.vs", "../OpenGl/resources/Shader/PBR/PBR.fs"));

		s_Data.LBL_RenderCube.reset(new Shader("../OpenGl/resources/Shader/LBL/CubeMap.vs", "../OpenGl/resources/Shader/LBL/equirectangularMap.fs"));
		s_Data.LBL_Sky.reset(new Shader("../OpenGl/resources/Shader/LBL/sky.vs", "../OpenGl/resources/Shader/LBL/sky.fs"));
		s_Data.LBL_Irradiance.reset(new Shader("../OpenGl/resources/Shader/LBL/CubeMap.vs", "../OpenGl/resources/Shader/LBL/Irradiance.fs"));
		s_Data.LBL_IrradianceSpecular.reset(new Shader("../OpenGl/resources/Shader/LBL/CubeMap.vs", "../OpenGl/resources/Shader/LBL/IrradianceSpecular.fs"));
		s_Data.LBL_BRDF.reset(new Shader("../OpenGl/resources/Shader/LBL/BRDF.vs", "../OpenGl/resources/Shader/LBL/BRDF.fs"));

		s_Data.LBL_ModelShader.reset(new Shader("../OpenGl/resources/Shader/PBR/ModelPBR.vs", "../OpenGl/resources/Shader/PBR/ModelPBR.fs"));

		s_Data.FontsShader.reset(new Shader("../OpenGl/resources/Shader/Fonts/fonts.vert", "../OpenGl/resources/Shader/Fonts/fonts.frag"));
		Renderer::SSAOKernel();

		//Texture
		s_Data.Texture1 = std::make_unique<Texture>("../OpenGl/resources/textures/container2.png");
		s_Data.Texture2 = std::make_unique<Texture>("../OpenGl/resources/textures/ChernoLogo.png");
		s_Data.Texture3 = std::make_unique<Texture>("../OpenGl/resources/textures/container2_specular.png");
		s_Data.grass_Texture = std::make_unique<Texture>("../OpenGl/resources/textures/grass.png");
		s_Data.window_Texture = std::make_unique<Texture>("../OpenGl/resources/textures/window.png");
		s_Data.metal_Texture = std::make_unique<Texture>("../OpenGl/resources/textures/metal.png", false);
		s_Data.metalGamma_Texture = std::make_unique<Texture>("../OpenGl/resources/textures/metal.png", true);

		s_Data.brickWall_Texture = std::make_unique<Texture>("../OpenGl/resources/textures/brickwall.jpg..png");
		s_Data.brickWall_Normal_Texture = std::make_unique<Texture>("../OpenGl/resources/textures/brickwall_normal.png");
		s_Data.brickWall_Depth_Texture = std::make_unique<Texture>("../OpenGl/resources/textures/brickwall.png");

		s_Data.cube_Texture = std::make_unique<Texture>();
		s_Data.cube_Texture->loadCubemap(s_Data.CubeTexturePath);

		s_Data.PBR_albedoTexture  = std::make_unique<Texture>("../OpenGl/resources/textures/PBR/ornate-brass_albedo.png");
		s_Data.PBR_aoTexture1 = std::make_unique<Texture>("../OpenGl/resources/textures/PBR/ornate-brass_ao.png");
		s_Data.PBR_metallicTexture2 = std::make_unique<Texture>("../OpenGl/resources/textures/PBR/ornate-brass_metallic.png");
		s_Data.PBR_normallTexture3 = std::make_unique<Texture>("../OpenGl/resources/textures/PBR/ornate-brass_normal-ogl.png");
		s_Data.PBR_roughnessTexture4 = std::make_unique<Texture>("../OpenGl/resources/textures/PBR/ornate-brass_roughness.png");
		//
		s_Data.LBL_HDRTexture = std::make_unique<Texture>();
		s_Data.LBL_HDRTexture->loadHDRMap("../OpenGl/resources/textures/HDR/newport_loft.hdr");

		s_Data.BackpackAOTexture = std::make_unique<Texture>("../OpenGl/resources/textures/PBR/ornate-brass_roughness.png");

		s_Data.LBL_CubeTexture = std::make_unique<Texture>();
		s_Data.LBL_CubeTexture->Cubemap(512, 512);
		s_Data.LBL_IrradianceTextureCube = std::make_unique<Texture>();
		s_Data.LBL_IrradianceTextureCube->Cubemap(32,32);
		s_Data.LBL_IrradianceSpecularTextureCube = std::make_unique<Texture>();
		s_Data.LBL_IrradianceSpecularTextureCube->MipmapCubemap();
		s_Data.LBL_BRDFTexture = std::make_unique<Texture>();
		s_Data.LBL_BRDFTexture->BRDFLUTMap();
		//Framebuffer

		FramebufferSpecification fbSpec1;
		fbSpec1.Attachments = { FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::RGBA16F,FramebufferTextureFormat::RGBA, FramebufferTextureFormat::Depth };
		s_Data.Multisample_FrameBuffer = std::make_unique<Framebuffer>(fbSpec1);
		s_Data.Multisample_FrameBuffer->InvalidateMRT();
		s_Data.Multisample_FrameBuffer->InitSSAO();

		s_Data.TextRendering = std::make_unique<TextRendering>();
		s_Data.TextRendering->init();
		s_Data.FontsShader->Bind();
		glm::mat4 projection = glm::ortho(0.0f, 1024.0f, 0.0f, 1024.0f);
		s_Data.FontsShader->SetMat4("projection", projection);

		//Model
		s_Data.m_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/nanosuit/nanosuit.obj");
		//s_Data.Gun_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/gun/Cerberus_LP.FBX");
		//s_Data.m_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/cyborg/cyborg.obj");
		s_Data.Gun_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/backpack/backpack.obj");
		//s_Data.m_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/planet/planet.obj");
		//s_Data.m_Model = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/vampire/vampire.obj");


		//init  DrawCall
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
		s_Data.m_DrawGamma = std::make_unique<DrawGamma>();
		s_Data.m_DrawGamma->Bind();
		s_Data.m_DrawTBNQuad = std::make_unique<DrawTBNQuad>();
		s_Data.m_DrawTBNQuad->Bind();
		s_Data.m_DrawSphere = std::make_unique<DrawSphere>();
		s_Data.m_DrawSphere->Bind();
		s_Data.m_DrawNewCube = std::make_unique<DrawNewCube>();
		s_Data.m_DrawNewCube->Bind();
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
		unsigned int uniformBlockIndex_GammaShader = glGetUniformBlockIndex(s_Data.GammaShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.GammaShader->GetShaderProgram(), uniformBlockIndex_GammaShader, 0);
		unsigned int uniformBlockIndex_Point_ShadowShader = glGetUniformBlockIndex(s_Data.Point_ShadowShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.Point_ShadowShader->GetShaderProgram(), uniformBlockIndex_Point_ShadowShader, 0);
		unsigned int uniformBlockIndex_TBNQuadShader = glGetUniformBlockIndex(s_Data.TBNQuadShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.TBNQuadShader->GetShaderProgram(), uniformBlockIndex_TBNQuadShader, 0);
		unsigned int uniformBlockIndex_HDRCubeShader = glGetUniformBlockIndex(s_Data.HDRCubeShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.HDRCubeShader->GetShaderProgram(), uniformBlockIndex_HDRCubeShader, 0);
		unsigned int uniformBlockIndex_G_BufferShader = glGetUniformBlockIndex(s_Data.G_BufferShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.G_BufferShader->GetShaderProgram(), uniformBlockIndex_G_BufferShader, 0);
		unsigned int uniformBlockIndex_PBR_Shader = glGetUniformBlockIndex(s_Data.PBR_Shader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.PBR_Shader->GetShaderProgram(), uniformBlockIndex_PBR_Shader, 0);
		unsigned int uniformBlockIndex_LBL_ModelShader = glGetUniformBlockIndex(s_Data.LBL_ModelShader->GetShaderProgram(), "Matrices");
		glUniformBlockBinding(s_Data.LBL_ModelShader->GetShaderProgram(), uniformBlockIndex_LBL_ModelShader, 0);

		s_Data.uniformBuffer = std::make_unique<Uniform>(sizeof(glm::mat4), 0);


		/////////////////////////////////////////////////////////////////////
		srand(300);
		for (unsigned int i = 0; i < 10; i++)
		{
			GLfloat rColor = ((rand() % 100) / 100.0f); // Between 0.5 and 1.0
			GLfloat gColor = ((rand() % 100) / 100.0f); // Between 0.5 and 1.0
			GLfloat bColor = ((rand() % 100) / 100.0f);
			s_Data.lightColors.push_back(glm::vec3(rColor, gColor, bColor));
		}
		// lighting info
		// -------------
		const unsigned int NR_LIGHTS = 32;

		srand(13);
		for (unsigned int i = 0; i < NR_LIGHTS; i++)
		{
			// calculate slightly random offsets
			float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
			float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
			float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
			Temp_LightPositions.push_back(glm::vec3(xPos, yPos, zPos));
			// also calculate random color
			float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
			float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
			float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
			Temp_LightColors.push_back(glm::vec3(rColor, gColor, bColor));
		}
		//texture->setInt///////////////////////////////////////////////////////////////////
		s_Data.CubeShader->Bind();
		s_Data.CubeShader->SetInt("material.texture1", 0);
		s_Data.CubeShader->SetInt("material.texture2", 1);
		s_Data.CubeShader->SetInt("material.specular_Texture", 2);
		s_Data.CubeShader->SetInt("material.shadowMap", 3);
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
		s_Data.GammaShader->Bind();

		s_Data.GammaShader->SetInt("floorTexture", 0);
		//
		s_Data.Point_ShadowShader->Bind();
		s_Data.Point_ShadowShader->SetInt("material.texture1", 0);
		s_Data.Point_ShadowShader->SetInt("material.texture2", 1);
		s_Data.Point_ShadowShader->SetInt("material.specular_Texture", 2);
		s_Data.Point_ShadowShader->SetInt("material.depthMap", 3);
		//
		s_Data.TBNQuadShader->Bind();
		s_Data.TBNQuadShader->SetInt("diffuseMap", 0);
		s_Data.TBNQuadShader->SetInt("normalMap", 1);
		s_Data.TBNQuadShader->SetInt("depthMap", 2);
		//
		s_Data.HDRShader->Bind();
		s_Data.HDRShader->SetInt("scene", 0);
		s_Data.HDRShader->SetInt("bloomBlur", 1);

		s_Data.HDRCubeShader->Bind();
		s_Data.HDRCubeShader->SetInt("diffuseTexture", 0);
		//
		s_Data.PingPongShader->Bind();
		s_Data.PingPongShader->SetInt("image", 0);
		//s_Data.MRTShader->Bind();
		//s_Data.MRTShader->SetInt("diffuseTexture", 0);
		s_Data.deferred_lightShader->Bind();
		s_Data.deferred_lightShader->SetInt("gPosition", 0);
		s_Data.deferred_lightShader->SetInt("gNormal", 1);
		s_Data.deferred_lightShader->SetInt("gAlbedoSpec", 2);

		//SSAO
		s_Data.SSAOLightShader->Bind();
		s_Data.SSAOLightShader->SetInt("gPosition", 0);
		s_Data.SSAOLightShader->SetInt("gNormal", 1);
		s_Data.SSAOLightShader->SetInt("gAlbedo", 2);
		s_Data.SSAOLightShader->SetInt("ssao", 3);

		s_Data.SAOShader->Bind();
		s_Data.SAOShader->SetInt("gPosition", 0);
		s_Data.SAOShader->SetInt("gNormal", 1);
		s_Data.SAOShader->SetInt("texNoise", 2);

		s_Data.SAOBlurShader->Bind();
		s_Data.SAOBlurShader->SetInt("ssaoInput", 0);

		//PBR/////////////////////////////////////////////////////////////
		s_Data.PBR_Shader->Bind();
		s_Data.PBR_Shader->SetInt("irradianceMap", 0);
		s_Data.PBR_Shader->SetInt("prefilterMap", 1);
		s_Data.PBR_Shader->SetInt("brdfLUT", 2);

		s_Data.PBR_Shader->SetInt("albedoMap", 3);
		s_Data.PBR_Shader->SetInt("normalMap", 4);
		s_Data.PBR_Shader->SetInt("metallicMap", 5);
		s_Data.PBR_Shader->SetInt("roughnessMap", 6);
		s_Data.PBR_Shader->SetInt("aoMap", 7);

		s_Data.LBL_ModelShader->Bind();
		s_Data.LBL_ModelShader->SetInt("irradianceMap", 4);
		s_Data.LBL_ModelShader->SetInt("prefilterMap", 5);
		s_Data.LBL_ModelShader->SetInt("brdfLUT", 6);
		s_Data.LBL_ModelShader->SetInt("aoMap", 7);
		//PBR预计算贴图//////////////////////////////////////////////////////////////////////////////////////////////////////////
		// pbr: setup framebuffer
		s_Data.LBL_FrameBuffer = std::make_unique<Framebuffer>();
		s_Data.LBL_FrameBuffer->InitRender();

		// 环境立方体
		// ----------------------------------------------------------------------
		s_Data.LBL_RenderCube->Bind();
		s_Data.LBL_RenderCube->SetMat4("projection", captureProjection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, s_Data.LBL_HDRTexture->GetRendererID());

		glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
		glBindFramebuffer(GL_FRAMEBUFFER, s_Data.LBL_FrameBuffer->GetLBLendererID());
		for (unsigned int i = 0; i < 6; ++i)
		{
			s_Data.LBL_RenderCube->SetMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, s_Data.LBL_CubeTexture->GetenvCubeTexture(), 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			s_Data.m_DrawNewCube->OnDraw(s_Data.LBL_RenderCube);//渲染立方体，一个摄像机方向，存储到当前绑定到帧缓冲的立方体贴图面
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glBindFramebuffer(GL_FRAMEBUFFER, s_Data.LBL_FrameBuffer->GetLBLendererID());
		glBindRenderbuffer(GL_RENDERBUFFER, s_Data.LBL_FrameBuffer->GetLBLRenderAttachmentendererID());
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

		//LBL_漫反射
		// -----------------------------------------------------------------------------
		s_Data.LBL_Irradiance->Bind();
		s_Data.LBL_Irradiance->SetMat4("projection", captureProjection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, s_Data.LBL_CubeTexture->GetenvCubeTexture());

		glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
		glBindFramebuffer(GL_FRAMEBUFFER, s_Data.LBL_FrameBuffer->GetLBLendererID());
		for (unsigned int i = 0; i < 6; ++i)
		{
			s_Data.LBL_Irradiance->SetMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, s_Data.LBL_IrradianceTextureCube->GetenvCubeTexture(), 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			s_Data.m_DrawNewCube->OnDraw(s_Data.LBL_Irradiance);//渲染立方体，一个摄像机方向，存储到当前绑定到帧缓冲的立方体贴图面
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// LBL_镜面反射
		// ----------------------------------------------------------------------------------------------------
		s_Data.LBL_IrradianceSpecular->Bind();
		s_Data.LBL_IrradianceSpecular->SetInt("environmentMap", 0);
		s_Data.LBL_IrradianceSpecular->SetMat4("projection", captureProjection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, s_Data.LBL_CubeTexture->GetenvCubeTexture());

		glBindFramebuffer(GL_FRAMEBUFFER, s_Data.LBL_FrameBuffer->GetLBLendererID());
		unsigned int maxMipLevels = 5;
		for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
		{
			// reisze framebuffer according to mip-level size.
			unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
			unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
			glBindRenderbuffer(GL_RENDERBUFFER, s_Data.LBL_FrameBuffer->GetLBLRenderAttachmentendererID());
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
			glViewport(0, 0, mipWidth, mipHeight);

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			//float roughness = 0.2;
			s_Data.LBL_IrradianceSpecular->SetFloat("roughness", roughness);
			for (unsigned int i = 0; i < 6; ++i)
			{
				s_Data.LBL_IrradianceSpecular->SetMat4("view", captureViews[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, s_Data.LBL_IrradianceSpecularTextureCube->GetMipmapCubeTexture(), mip);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				s_Data.m_DrawNewCube->OnDraw(s_Data.LBL_IrradianceSpecular);
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//BRDF
		// ----------------------------------------------------------------------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, s_Data.LBL_FrameBuffer->GetLBLendererID());
		glBindRenderbuffer(GL_RENDERBUFFER, s_Data.LBL_FrameBuffer->GetLBLRenderAttachmentendererID());
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, s_Data.LBL_BRDFTexture->GetBRDFTexture(), 0);

		glViewport(0, 0, 512, 512);
		s_Data.LBL_BRDF->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		s_Data.m_DrawScreenQuad->OnDraw(s_Data.LBL_BRDF);


		//重置
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 1024,1024);
	}
	void Renderer::EndScene()
	{
		glCheckError();
		//input
		processInput(App::Get().GetWindow().GetNativeWindow());
		//std::cout << glGetError() << std::endl;
		//init
		Renderer::Clear();//需要放在这个位置，清除自己的帧缓冲
		//
		glm::mat4 model = glm::mat4(1.0f);
		//GreenValue
		float timeValue = glfwGetTime();
		float GreenValue = (sin(timeValue) / 2.0f) + 0.5f;//sin值变为（-1——1），/2+0.5-》0——1
		glm::vec4 result = glm::vec4(0.0f, GreenValue, 0.0f, 1.0f);
		glCheckError();
		/// <summary>
		///平躺人物模型 ： SSAO + 延迟渲染
		/// </summary>
		//G_FrameBuffer///////////////////////////////////////////////////////////////////////////
		//G_FrameBuffer///////////////////////////////////////////////////////////////////////////
		//G_FrameBuffer///////////////////////////////////////////////////////////////////////////

		s_Data.Multisample_FrameBuffer->BindMRTFramebuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = App::Get().GetCamera().GetProjection();
		glm::mat4 view = App::Get().GetCamera().GetViewMatrix();

		s_Data.G_BufferSSAOShader->Bind();
		s_Data.G_BufferSSAOShader->SetMat4("projection", projection);
		s_Data.G_BufferSSAOShader->SetMat4("view", view);
		model = glm::scale(model, glm::vec3(0.3f));
		model = glm::rotate(model, glm::radians(-90.0f),glm::vec3(0.0, 1.0, 0.0)) 
			* glm::rotate(glm::mat4(1.0f),
			glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, -10.0f, -12.0));
		s_Data.G_BufferSSAOShader->SetMat4("model", model);
		s_Data.G_BufferSSAOShader->SetInt("invertedNormals", 0);
		s_Data.m_Model->Draw(s_Data.G_BufferSSAOShader);


		s_Data.G_BufferCubeSSAOShader->Bind();
		model = glm::scale(glm::mat4(1.0f), glm::vec3(8.0f,0.1f,3.0f));
		model = glm::translate(model, glm::vec3(0.0f, -40.0f, 0.0));
		s_Data.G_BufferCubeSSAOShader->SetMat4("projection", projection);
		s_Data.G_BufferCubeSSAOShader->SetMat4("view", view);
		s_Data.G_BufferCubeSSAOShader->SetMat4("model", model);
		s_Data.G_BufferSSAOShader->SetInt("invertedNormals", 0);
		s_Data.m_DrawCube->OnDraw(s_Data.G_BufferCubeSSAOShader);


		s_Data.Multisample_FrameBuffer->Unbind();

		glCheckError();
		//SSAO///////////////////////////////////////////////////////////////////////////
		//SSAO///////////////////////////////////////////////////////////////////////////
		//SSAO///////////////////////////////////////////////////////////////////////////
		s_Data.Multisample_FrameBuffer->BindSSAOFramebuffer();
		glClear(GL_COLOR_BUFFER_BIT);
		s_Data.SAOShader->Bind();
		
		for (unsigned int i = 0; i < 64; ++i) {
		
			s_Data.SAOShader->SetFloat3("samples[" + std::to_string(i) + "]", s_Data.ssaoKernel[i]);
		}
		s_Data.SAOShader->SetMat4("projection", projection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, s_Data.Multisample_FrameBuffer->GetMRTAttachmentRendererID(0));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, s_Data.Multisample_FrameBuffer->GetMRTAttachmentRendererID(1));
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		s_Data.m_DrawScreenQuad->OnDraw(s_Data.SAOShader);

		s_Data.Multisample_FrameBuffer->Unbind();
		glCheckError();
		//Blur///////////////////////////////////////////////////////////////////////////
		//Blur///////////////////////////////////////////////////////////////////////////
		//Blur///////////////////////////////////////////////////////////////////////////
		s_Data.Multisample_FrameBuffer->BindSSAOBlurFramebuffer();
		glClear(GL_COLOR_BUFFER_BIT);
		s_Data.SAOBlurShader->Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, s_Data.Multisample_FrameBuffer->GetSSAOColorBufferAttachmentID());
		s_Data.m_DrawScreenQuad->OnDraw(s_Data.SAOBlurShader);

		s_Data.Multisample_FrameBuffer->Unbind();
		glCheckError();
		//deferred_Light///////////////////////////////////////////////////////////////////////////
		//deferred_Light///////////////////////////////////////////////////////////////////////////
		//deferred_Light///////////////////////////////////////////////////////////////////////////ss
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		s_Data.SSAOLightShader->Bind();
		glm::vec3 lightPosView = glm::vec3(view * glm::vec4(2.0, 4.0, -2.0, 1.0));
		s_Data.SSAOLightShader->SetFloat3("light.Position", lightPosView);
		s_Data.SSAOLightShader->SetFloat3("light.Color", glm::vec3(0.7f,0.7f,1.0f));
		// Update attenuation parameters
		const float linear = 0.09f;
		const float quadratic = 0.032f;
		s_Data.SSAOLightShader->SetFloat("light.Linear", linear);
		s_Data.SSAOLightShader->SetFloat("light.Quadratic", quadratic);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, s_Data.Multisample_FrameBuffer->GetMRTAttachmentRendererID(0));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, s_Data.Multisample_FrameBuffer->GetMRTAttachmentRendererID(1));
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, s_Data.Multisample_FrameBuffer->GetMRTAttachmentRendererID(2));
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, s_Data.Multisample_FrameBuffer->GetSSAOColorBufferBlurAttachmentID());
		s_Data.m_DrawScreenQuad->OnDraw(s_Data.SSAOLightShader);

		//s_Data.Multisample_FrameBuffer->Unbind();
		glCheckError();


		//glBlitFramebuffer////////////////////////////////////////////////////////////////
		//glBlitFramebuffer////////////////////////////////////////////////////////////////
		//glBlitFramebuffer////////////////////////////////////////////////////////////////

		glBindFramebuffer(GL_READ_FRAMEBUFFER, s_Data.Multisample_FrameBuffer->GetMRTRendererID());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, 1024, 1024, 0, 0, 1024, 1024, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		///PBR球体
		//PBR_Model///////////////////////////////////////////////////////////////////////////
		//PBR_Model///////////////////////////////////////////////////////////////////////////
		//PBR_Model///////////////////////////////////////////////////////////////////////////
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, s_Data.LBL_IrradianceTextureCube->GetenvCubeTexture());
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_CUBE_MAP, s_Data.LBL_IrradianceSpecularTextureCube->GetMipmapCubeTexture());
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, s_Data.LBL_BRDFTexture->GetBRDFTexture());
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, s_Data.BackpackAOTexture->GetRendererID());

		s_Data.LBL_ModelShader->Bind();
		s_Data.LBL_ModelShader->SetFloat3("camPos", App::Get().GetCamera().GetPosition());
		s_Data.LBL_ModelShader->SetFloat3("albedo", glm::vec3(0.0f, 0.5f, 0.0f));
		//s_Data.LBL_ModelShader->SetFloat("ao", 1.0f);

		//
		model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-10.0f, 25.0f, 0.0f));
		s_Data.LBL_ModelShader->SetMat4("model", model);
		s_Data.LBL_ModelShader->SetMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		s_Data.Gun_Model->Draw(s_Data.LBL_ModelShader);


		//绘制灯
		for (unsigned int i = 0; i < sizeof(PBR_lightPositions) / sizeof(PBR_lightPositions[0]); ++i)
		{
			glm::vec3 newPos = PBR_lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			//newPos = PBR_lightPositions[i];
			s_Data.LBL_ModelShader->SetFloat3("lightPositions[" + std::to_string(i) + "]", newPos);
			s_Data.LBL_ModelShader->SetFloat3("lightColors[" + std::to_string(i) + "]", PBR_lightColors[i]);

			model = glm::mat4(1.0f);
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			s_Data.LBL_ModelShader->SetMat4("model", model);
			s_Data.LBL_ModelShader->SetMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		}

		glCheckError();
		///PBR背包
		//PBR///////////////////////////////////////////////////////////////////////////
		//PBR///////////////////////////////////////////////////////////////////////////
		//PBR///////////////////////////////////////////////////////////////////////////
		s_Data.PBR_Shader->Bind();
		s_Data.PBR_Shader->SetFloat3("camPos", App::Get().GetCamera().GetPosition());
		s_Data.PBR_Shader->SetFloat3("albedo", glm::vec3 (0.0f, 0.5f, 0.0f));
		s_Data.PBR_Shader->SetFloat("ao", 1.0f);

		//绘制球体，49个
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, s_Data.LBL_IrradianceTextureCube->GetenvCubeTexture());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, s_Data.LBL_IrradianceSpecularTextureCube->GetMipmapCubeTexture());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, s_Data.LBL_BRDFTexture->GetBRDFTexture());


		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, s_Data.PBR_albedoTexture->GetRendererID()); 
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, s_Data.PBR_normallTexture3->GetRendererID());
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, s_Data.PBR_metallicTexture2->GetRendererID());
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, s_Data.PBR_roughnessTexture4->GetRendererID());
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, s_Data.PBR_aoTexture1->GetRendererID());

		for (int row = 0; row < nrRows; ++row)
		{
			s_Data.PBR_Shader->SetFloat("metallic", (float)row / (float)nrRows);//金属色，每行的金属度一致（0---1），越往下越是金属
			for (int col = 0; col < nrColumns; ++col)
			{
				// we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
				// on direct lighting.
				//粗糙度，每列的粗糙度一致，越往右越粗糙
				s_Data.PBR_Shader->SetFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

				//model = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f,0.2f,0.2f));
				//-8 ---8，间隔为2.5
				model = glm::translate(glm::mat4(1.0f), glm::vec3(
					(float)(col - (nrColumns / 2)) * spacing ,
					(float)(row - (nrRows / 2)) * spacing + 20.0f
					,2.0f)
				);
				s_Data.PBR_Shader->SetMat4("model", model);
				s_Data.PBR_Shader->SetMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
				s_Data.m_DrawSphere->OnDraw(s_Data.PBR_Shader);
			}
		}
		//绘制灯
		for (unsigned int i = 0; i < sizeof(PBR_lightPositions) / sizeof(PBR_lightPositions[0]); ++i)
		{
			glm::vec3 newPos = PBR_lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			//newPos = PBR_lightPositions[i];
			s_Data.PBR_Shader->SetFloat3("lightPositions[" + std::to_string(i) + "]", newPos);
			s_Data.PBR_Shader->SetFloat3("lightColors[" + std::to_string(i) + "]", PBR_lightColors[i]);

			model = glm::mat4(1.0f);
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			s_Data.PBR_Shader->SetMat4("model", model);
			s_Data.PBR_Shader->SetMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
			s_Data.m_DrawSphere->OnDraw(s_Data.PBR_Shader);
		}

		glCheckError();
		///Gamma校正，平面和光
		//Gamma///////////////////////////////////////////////////////////////////////////
		//Gamma///////////////////////////////////////////////////////////////////////////
		//Gamma///////////////////////////////////////////////////////////////////////////
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gammaEnabled ? s_Data.metalGamma_Texture->GetRendererID() : s_Data.metal_Texture->GetRendererID());
		s_Data.GammaShader->Bind();
		glUniform3fv(glGetUniformLocation(s_Data.GammaShader->GetShaderProgram(), "lightPositions"), 4, &lightPositions[0][0]);
		glUniform3fv(glGetUniformLocation(s_Data.GammaShader->GetShaderProgram(), "lightColors"), 4, &lightColors[0][0]);
		s_Data.GammaShader->SetFloat3("viewPos", App::Get().GetCamera().GetPosition());
		s_Data.GammaShader->SetInt("gamma", gammaEnabled);
		model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, 35.0f));
		s_Data.GammaShader->SetMat4("model", model);
		s_Data.m_DrawGamma->OnDraw(s_Data.GammaShader);

		///凹凸贴图，中心旋转立方体
		//TBN_Quad_BrickWall///////////////////////////////////////////////////////////////////////////
		//TBN_Quad_BrickWall///////////////////////////////////////////////////////////////////////////
		//TBN_Quad_BrickWall///////////////////////////////////////////////////////////////////////////
		s_Data.TBNQuadShader->Bind();
		model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		model = glm::rotate(model, (GLfloat)glfwGetTime() * -0.1f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.1f));
		s_Data.TBNQuadShader->SetMat4("model", model);
		s_Data.TBNQuadShader->SetFloat3("lightPos", lightPos);
		s_Data.TBNQuadShader->SetFloat3("viewPos", App::Get().GetCamera().GetPosition());
		glActiveTexture(GL_TEXTURE0);
		s_Data.brickWall_Texture->Bind();
		glActiveTexture(GL_TEXTURE1);
		s_Data.brickWall_Normal_Texture->Bind();
		glActiveTexture(GL_TEXTURE2);
		s_Data.brickWall_Depth_Texture->Bind();
		s_Data.m_DrawTBNQuad->OnDraw(s_Data.TBNQuadShader);

		glCheckError();
		///几何，平面房子
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
		///行星，实例化
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

		glCheckError();
		///天空盒
		//skybox///////////////////////////////////////////////////////////////////////////
		//skybox///////////////////////////////////////////////////////////////////////////
		//skybox///////////////////////////////////////////////////////////////////////////
		{
			//glActiveTexture(GL_TEXTURE0);
			//s_Data.cube_Texture->BindCubeTexture();
			//glDepthFunc(GL_LEQUAL);
			//s_Data.SkyboxShader->Bind();

			//glm::mat4 view = glm::mat4(glm::mat3(App::Get().GetCamera().GetViewMatrix()));
			//glm::mat4 projection = App::Get().GetCamera().GetProjection();
			//s_Data.SkyboxShader->SetMat4("view", view);
			//s_Data.SkyboxShader->SetMat4("projection", projection);

			//s_Data.m_DrawSkybox->OnDraw(s_Data.SkyboxShader);
			//s_Data.SkyboxShader->Bind();
			//glDepthFunc(GL_LESS);
		}
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, s_Data.LBL_CubeTexture->GetenvCubeTexture());
			glDepthFunc(GL_LEQUAL);
			s_Data.LBL_Sky->Bind();
			glm::mat4 view = App::Get().GetCamera().GetViewMatrix();
			glm::mat4 projection = App::Get().GetCamera().GetProjection();
			s_Data.LBL_Sky->SetMat4("projection", projection);
			s_Data.LBL_Sky->SetMat4("view", view);
			s_Data.m_DrawNewCube->OnDraw(s_Data.LBL_Sky);
			glDepthFunc(GL_LESS);
		}
		///草，混合
		//grass///////////////////////////////////////////////////////////////////////////
		//grass///////////////////////////////////////////////////////////////////////////
		//grass///////////////////////////////////////////////////////////////////////////
		glActiveTexture(GL_TEXTURE0);
		s_Data.grass_Texture->Bind();

		s_Data.QuadShader->Bind();
		model = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 0.0f));
		s_Data.QuadShader->SetFloat4("ourColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		s_Data.QuadShader->SetMat4("model", model);

		s_Data.m_DrawQuad->OnDraw(s_Data.QuadShader);

		//lightPos.z = static_cast<float>(sin(glfwGetTime() * 0.5) * 3.0);
		// 0. create depth cubemap transformation matrices
		// -----------------------------------------------
		Renderer::DrawScene();

		///光源
		//Renderer_Scene
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
			s_Data.PointLightShader->SetFloat3("color", lightColors[i]);
			s_Data.m_DrawPointLight->OnDraw(s_Data.PointLightShader);

		}
		///线框
		// point /Line  //////////////////////////////////////////////////////////////////////////////
		// point /Line  //////////////////////////////////////////////////////////////////////////////
		// point /Line  //////////////////////////////////////////////////////////////////////////////
		s_Data.PointShader->Bind();
		model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 10.0f));
		s_Data.PointShader->SetMat4("model", model);
		s_Data.PointShader->SetFloat3("color", result);
		s_Data.m_DrawPoint->OnDraw(s_Data.PointShader);

		//modle//////////////////////////////////////////////////////////////////////////////////
		//modle//////////////////////////////////////////////////////////////////////////////////
		//modle//////////////////////////////////////////////////////////////////////////////////
		///人物模型，法线，凹凸贴图
		s_Data.PlanetShader->Bind();

		//
		s_Data.PlanetShader->SetFloat("shininess", 32.0f);
		//
		s_Data.PlanetShader->SetFloat3("constVal.camera_Position", App::Get().GetCamera().GetPosition());
		s_Data.PlanetShader->SetFloat3("constVal.camera_Direction", App::Get().GetCamera().GetForwardDirection());
		s_Data.PlanetShader->SetFloat("constVal.constant", 1.0f);
		s_Data.PlanetShader->SetFloat("constVal.linear", 0.09f);
		s_Data.PlanetShader->SetFloat("constVal.quadratic", 0.032f);
		//

		// directional light
		s_Data.PlanetShader->SetFloat3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		s_Data.PlanetShader->SetFloat3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		s_Data.PlanetShader->SetFloat3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		s_Data.PlanetShader->SetFloat3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		// point light 1
		for (int i = 0; i < 10; i++) {
			s_Data.PlanetShader->SetFloat3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
			s_Data.PlanetShader->SetFloat3("pointLights[" + std::to_string(i) + "].color", s_Data.lightColors[i]);

			s_Data.PlanetShader->SetFloat3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			s_Data.PlanetShader->SetFloat3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			s_Data.PlanetShader->SetFloat3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		}

		// spotLight

		s_Data.PlanetShader->SetFloat3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		s_Data.PlanetShader->SetFloat3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		s_Data.PlanetShader->SetFloat3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		s_Data.PlanetShader->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		s_Data.PlanetShader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));



		//
		model = glm::scale(glm::mat4(1.0f), glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::translate(model, glm::vec3(20.0f, -10.0f, 0.0f));
		s_Data.PlanetShader->SetMat4("model", model);

		s_Data.m_Model->Draw(s_Data.PlanetShader);

		glCheckError();
		///窗户，混合
		//window///////////////////////////////////////////////////////////////////////////
		//window///////////////////////////////////////////////////////////////////////////
		//window///////////////////////////////////////////////////////////////////////////
		glEnable(GL_BLEND);
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
		glDisable(GL_BLEND);

		glCheckError();


		//FBO——WIN//////////////////////////////////////////////////////////////////////////////		
		//FBO——WIN//////////////////////////////////////////////////////////////////////////////
		//FBO——WIN//////////////////////////////////////////////////////////////////////////////
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
		glViewport(1024 / 4 * 3, 1024 / 4 * 3, 1024 / 4, 1024 / 4);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, s_Data.Multisample_FrameBuffer->GetMRTAttachmentRendererID(1));
		s_Data.SceneShader->Bind();
		s_Data.m_DrawScreenQuad->Bind();

		s_Data.m_DrawScreenQuad->OnDraw(s_Data.SceneShader);

		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, 1024, 1024);
		// //pingpong//////////////////////////////////////////////////////////////////////////////
		//pingpong//////////////////////////////////////////////////////////////////////////////
		//pingpong//////////////////////////////////////////////////////////////////////////////
		//

		//bool horizontal = true, first_iteration = true;
		//unsigned int amount = 10;
		//s_Data.PingPongShader->Bind();
		//for (unsigned int i = 0; i < amount; i++)
		//{
		//	glBindFramebuffer(GL_FRAMEBUFFER, s_Data.Multisample_FrameBuffer->GetPingPongRendererID(horizontal));
		//	s_Data.PingPongShader->SetInt("horizontal", horizontal);
		//	glBindTexture(GL_TEXTURE_2D, first_iteration ? s_Data.Multisample_FrameBuffer->GetMRTAttachmentRendererID(1) : s_Data.Multisample_FrameBuffer->GetPingPongAttachmentRendererID(!horizontal));  // bind texture of other framebuffer (or scene if first iteration)
		//	s_Data.m_DrawScreenQuad->OnDraw(s_Data.PingPongShader);
		//	horizontal = !horizontal;
		//	if (first_iteration)
		//		first_iteration = false;
		//}
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//LUT//////////////////////////////////////////////////////////////////////////////
		//LUT//////////////////////////////////////////////////////////////////////////////
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
		//glDisable(GL_DEPTH_TEST);
		//glBindTexture(GL_TEXTURE_2D, s_Data.LBL_BRDFTexture->GetBRDFTexture());
		//s_Data.SceneShader->Bind();
		//s_Data.m_DrawScreenQuad->Bind();

		//s_Data.m_DrawScreenQuad->OnDraw(s_Data.SceneShader);
		//Fonts//////////////////////////////////////////////////////////////////////////////	
		//Fonts//////////////////////////////////////////////////////////////////////////////
		//Fonts//////////////////////////////////////////////////////////////////////////////
		s_Data.FontsShader->Bind();
		s_Data.TextRendering->RenderText(*s_Data.FontsShader, "This is SenLongAn's project", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		s_Data.TextRendering->RenderText(*s_Data.FontsShader, "Hello World!", 800.0f, 700.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

	}
	void Renderer::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void Renderer::Clear()
	{
		//深度测试
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		//模板测试
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		//混合
		//glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//点渲染模式，调整大小
		glEnable(GL_PROGRAM_POINT_SIZE);
		//清除颜色缓冲
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		glCheckError();

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

	void Renderer::DrawScene()
	{
		//plane地面//////////////////////////////////////////////////////////////////////////////////
		//plane地面//////////////////////////////////////////////////////////////////////////////////
		//plane地面//////////////////////////////////////////////////////////////////////////////////

		glm::mat4 model = glm::mat4(1.0f);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		glActiveTexture(GL_TEXTURE0);
		s_Data.metal_Texture->Bind();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, 0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, s_Data.Shadow_FrameBuffer->GetDepthCubeAttachmentRendererID());

		s_Data.Point_ShadowShader->Bind();

		s_Data.Point_ShadowShader->SetFloat("material.shininess", 32.0f);
		//
		s_Data.Point_ShadowShader->SetFloat3("constVal.camera_Position", App::Get().GetCamera().GetPosition());
		s_Data.Point_ShadowShader->SetFloat3("constVal.camera_Direction", App::Get().GetCamera().GetForwardDirection());
		s_Data.Point_ShadowShader->SetFloat("constVal.constant", 1.0f);
		s_Data.Point_ShadowShader->SetFloat("constVal.linear", 0.09f);
		s_Data.Point_ShadowShader->SetFloat("constVal.quadratic", 0.032f);

		s_Data.Point_ShadowShader->SetFloat("constVal.blinn", blinn);
		s_Data.Point_ShadowShader->SetFloat("constVal.gamma", gammaEnabled);

		s_Data.Point_ShadowShader->SetFloat3("constVal.lightPos", lightPos);
		s_Data.Point_ShadowShader->SetInt("constVal.shadows", shadows);
		s_Data.Point_ShadowShader->SetFloat("constVal.far_plane", far_plane);
		//

		// directional light
		s_Data.Point_ShadowShader->SetFloat3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		s_Data.Point_ShadowShader->SetFloat3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		s_Data.Point_ShadowShader->SetFloat3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		s_Data.Point_ShadowShader->SetFloat3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		// point light 1
		for (int i = 0; i < 10; i++) {
			s_Data.Point_ShadowShader->SetFloat3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
			s_Data.Point_ShadowShader->SetFloat3("pointLights[" + std::to_string(i) + "].color", s_Data.lightColors[i]);

			s_Data.Point_ShadowShader->SetFloat3("pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			s_Data.Point_ShadowShader->SetFloat3("pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			s_Data.Point_ShadowShader->SetFloat3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		}

		// spotLight

		s_Data.Point_ShadowShader->SetFloat3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		s_Data.Point_ShadowShader->SetFloat3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		s_Data.Point_ShadowShader->SetFloat3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		s_Data.Point_ShadowShader->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		s_Data.Point_ShadowShader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		//
		model = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 1.0f, 50.0f));
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, -0.1f));
		s_Data.Point_ShadowShader->SetMat4("model", model);
		s_Data.m_DrawCube->OnDraw(s_Data.Point_ShadowShader);
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
		glBindTexture(GL_TEXTURE_2D, 0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, s_Data.Shadow_FrameBuffer->GetDepthCubeAttachmentRendererID());
		//
		s_Data.Point_ShadowShader->Bind();
		//		//
		for (unsigned int i = 0; i < 10; i++)
		{
			model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			float angle = 20.0f * i + 1;
			model = glm::rotate(model, glm::radians(40.0f), glm::vec3(1.0f, 0.3f, 0.5f));
			s_Data.Point_ShadowShader->SetMat4("model", model);

			//Renderer::DrawIndexed(s_Data.CubeVertexArray);
			s_Data.m_DrawCube->OnDraw(s_Data.Point_ShadowShader);
		}

		glCheckError();
	}

	void Renderer::processInput(GLFWwindow* window)
	{
		//Gamma
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !gammaKeyPressed)
		{
			gammaEnabled = !gammaEnabled;//是否启用
			gammaKeyPressed = true;

		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)//松开按键执行
		{
			gammaKeyPressed = false;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !shadowsKeyPressed)
		{
			shadows = !shadows;
			shadowsKeyPressed = true;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		{
			shadowsKeyPressed = false;
		}
	}

	void Renderer::SSAOKernel()
	{
		// generate sample kernel
		// ----------------------
		std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
		std::default_random_engine generator;
		for (unsigned int i = 0; i < 64; ++i)
		{
			glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
			sample = glm::normalize(sample);
			sample *= randomFloats(generator);
			float scale = float(i) / 64.0f;

			// scale samples s.t. they're more aligned to center of kernel
			scale = ourLerp(0.1f, 1.0f, scale * scale);
			sample *= scale;
			s_Data.ssaoKernel.push_back(sample);
		}

		// generate noise texture
		// ----------------------
		for (unsigned int i = 0; i < 16; i++)
		{
			glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
			s_Data.ssaoNoise.push_back(noise);
		}
		glGenTextures(1, &noiseTexture);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &s_Data.ssaoNoise[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}


}