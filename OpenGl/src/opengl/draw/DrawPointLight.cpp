#include "DrawPointLight.h"

namespace Opengl {
	//cube(postion)
	float pointLight_Vertices[] = {//后，前，左，右，下，上（左下，右下，右上，右上，左上，左下）
		// back face
	   -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
	   // front face		  			   
	   -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
	   -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
	   // left face		  			   
	   -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
	   // right face		  			   
		0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
		// bottom face		  			   
	   -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
	   -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
	   -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
	   // top face			  			   
	   -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
		0.5f,  0.5f , 0.5f,   1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
	   -0.5f,  0.5f,  0.5f,   0.0f, 0.0f
	};
	//cube
	//unsigned int pointLight_Indices[36] = {
	//	0, 1, 2, 2, 3, 0 ,
	//	4,5,6,6,7,4,
	//	8, 9, 10, 10, 11, 8,
	//	12, 13, 14, 14, 15, 12,
	//	16, 17, 18, 18, 19, 16,
	//	20, 21, 22, 22, 23, 20
	//};

	struct pointLightData {
		std::shared_ptr<VertexArray> pointLightVertexArray;
	};
	static pointLightData s_pointLightData;

	void DrawPointLight::Bind() const
	{
		s_pointLightData.pointLightVertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> pointLight_VertexBuffer;
		pointLight_VertexBuffer.reset(VertexBuffer::Create(pointLight_Vertices, sizeof(pointLight_Vertices)));

		pointLight_VertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float2, "a_Normal" },
			}
		);

		s_pointLightData.pointLightVertexArray->AddVertexBuffer(pointLight_VertexBuffer);

		//std::shared_ptr<IndexBuffer> pointLight_IndexBuffer;
		//pointLight_IndexBuffer.reset(IndexBuffer::Create(pointLight_Indices, sizeof(pointLight_Indices) / sizeof(unsigned int)));

		//s_pointLightData.pointLightVertexArray->SetIndexBuffer(pointLight_IndexBuffer);

	}
	void DrawPointLight::OnDraw(const std::shared_ptr<Shader>& shader) const
	{
		s_pointLightData.pointLightVertexArray->Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//Renderer::DrawIndexed(s_pointLightData.pointLightVertexArray);
		s_pointLightData.pointLightVertexArray->Unbind();
	}
}