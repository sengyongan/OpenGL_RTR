#include "DrawPoint.h"

namespace Opengl {
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
	//cube
	unsigned int Point_Indices[36] = {
		0, 1, 2, 2, 3, 0 ,
		4,5,6,6,7,4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20
	};

	struct PointData {
		std::shared_ptr<VertexArray> PointVertexArray;
	};
	static PointData s_PointData;

	void DrawPoint::Bind() const
	{
		s_PointData.PointVertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> point_VertexBuffer;
		point_VertexBuffer.reset(VertexBuffer::Create(point_Vertices, sizeof(point_Vertices)));

		point_VertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
			}
		);

		s_PointData.PointVertexArray->AddVertexBuffer(point_VertexBuffer);

		std::shared_ptr<IndexBuffer> point_IndexBuffer;
		point_IndexBuffer.reset(IndexBuffer::Create(Point_Indices, sizeof(Point_Indices) / sizeof(unsigned int)));

		s_PointData.PointVertexArray->SetIndexBuffer(point_IndexBuffer);

	}
	void DrawPoint::OnDraw(const std::shared_ptr<Shader>& shader) const
	{
		s_PointData.PointVertexArray->Bind();
		Renderer::DrawIndexed(s_PointData.PointVertexArray);
	}
}