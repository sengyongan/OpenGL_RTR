#include "DrawSkybox.h"

namespace Opengl {
//		// positions          
//	-1.0f,  1.0f, -1.0f,235562
//	-1.0f, -1.0f, -1.0f,
//	 1.0f, -1.0f, -1.0f,
//	 1.0f, -1.0f, -1.0f,
//	 1.0f,  1.0f, -1.0f,
//	-1.0f,  1.0f, -1.0f,
//————————————————
//
//							版权声明：本文为博主原创文章，遵循 CC 4.0 BY - SA 版权协议，转载请附上原文出处链接和本声明。
//
//原文链接：https ://blog.csdn.net/weixin_43803133/article/details/108415427
	//cube(postion)
	float Skybox_Vertices[] = {//后，前，左，右，下，上（左下，右下，右上，右上，左上，左下）
		// positions          
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	//cube
	//unsigned int Skybox_Indices[36] = {
	//	0, 1, 2, 2, 3, 0 ,
	//	4, 5, 6, 6, 7, 4,
	//	8, 9, 10, 10, 11, 8,
	//	12, 13, 14, 14, 15, 12,
	//	16, 17, 18, 18, 19, 16,
	//	20, 21, 22, 22, 23, 20
	//};

	struct SkyboxData {
		std::shared_ptr<VertexArray> SkyboxVertexArray;
	};
	static SkyboxData s_SkyboxData;

	void DrawSkybox::Bind() const
	{
		s_SkyboxData.SkyboxVertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> Skybox_VertexBuffer;
		Skybox_VertexBuffer.reset(VertexBuffer::Create(Skybox_Vertices, sizeof(Skybox_Vertices)));

		Skybox_VertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" }
			}
		);

		s_SkyboxData.SkyboxVertexArray->AddVertexBuffer(Skybox_VertexBuffer);

		//std::shared_ptr<IndexBuffer> Skybox_IndexBuffer;
		//Skybox_IndexBuffer.reset(IndexBuffer::Create(Skybox_Indices, sizeof(Skybox_Indices) / sizeof(unsigned int)));

		//s_SkyboxData.SkyboxVertexArray->SetIndexBuffer(Skybox_IndexBuffer);

	}
	void DrawSkybox::OnDraw(const std::shared_ptr<Shader>& shader) const
	{
		s_SkyboxData.SkyboxVertexArray->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		s_SkyboxData.SkyboxVertexArray->Unbind();

	}
}