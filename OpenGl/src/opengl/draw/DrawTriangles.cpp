#include "DrawTriangles.h"
#include "DrawTriangles.h"

namespace Opengl {
	// Triangles
	float triangles_vertices[3 * 7] = {//position + color
		-0.5f, -0.5f, 0.0f,		0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f,		0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f,		0.8f, 0.8f, 0.2f, 1.0f
	};
	//Triangles
	unsigned int triangles_Indices[3] = { 0, 1, 2 };


	struct TrianglesData {
		std::shared_ptr<VertexArray> TrianglesVertexArray;
	};
	static TrianglesData s_TrianglesData;

	void DrawTriangles::Bind() const
	{
		s_TrianglesData.TrianglesVertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> point_VertexBuffer;
		point_VertexBuffer.reset(VertexBuffer::Create(triangles_vertices, sizeof(triangles_vertices)));

		point_VertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
			}
		);

		s_TrianglesData.TrianglesVertexArray->AddVertexBuffer(point_VertexBuffer);

		std::shared_ptr<IndexBuffer> point_IndexBuffer;
		point_IndexBuffer.reset(IndexBuffer::Create(triangles_Indices, sizeof(triangles_Indices) / sizeof(unsigned int)));

		s_TrianglesData.TrianglesVertexArray->SetIndexBuffer(point_IndexBuffer);

	}
	void DrawTriangles::OnDraw(const std::shared_ptr<Shader>& shader) const
	{
		s_TrianglesData.TrianglesVertexArray->Bind();
		Renderer::DrawIndexed(s_TrianglesData.TrianglesVertexArray);
	}
}