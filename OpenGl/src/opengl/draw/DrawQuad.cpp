#include "DrawQuad.h"

namespace Opengl {
	//quad
	float squareVertices[] = {//position + texture左下，右下，右上，左上
		-0.75f, -0.75f, 0.0f,	0.0f, 0.0f,
		 0.75f, -0.75f, 0.0f,	1.0f, 0.0f,
		 0.75f,  0.75f, 0.0f,	1.0f, 1.0f,
		-0.75f,  0.75f, 0.0f,	0.0f, 1.0f
	};
	//quad
	unsigned int square_Indices[6] = { 0, 1, 2, 2, 3, 0 };


	struct QuadData {
		std::shared_ptr<VertexArray> QuadVertexArray;
	};
	static QuadData s_QuadData;

	void DrawQuad::Bind() const
	{
		s_QuadData.QuadVertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> point_VertexBuffer;
		point_VertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		point_VertexBuffer->SetLayout(
			{
					{ ShaderDataType::Float3, "a_Position" },
					{ ShaderDataType::Float2, "a_TexCoord" }
			}
		);

		s_QuadData.QuadVertexArray->AddVertexBuffer(point_VertexBuffer);

		std::shared_ptr<IndexBuffer> point_IndexBuffer;
		point_IndexBuffer.reset(IndexBuffer::Create(square_Indices, sizeof(square_Indices) / sizeof(unsigned int)));

		s_QuadData.QuadVertexArray->SetIndexBuffer(point_IndexBuffer);

	}
	void DrawQuad::OnDraw(const std::shared_ptr<Shader>& shader) const
	{
		s_QuadData.QuadVertexArray->Bind();
		Renderer::DrawIndexed(s_QuadData.QuadVertexArray);
	}
}