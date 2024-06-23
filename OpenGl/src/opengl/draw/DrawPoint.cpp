#include "DrawPoint.h"

namespace Opengl {
	float point_Vertices[] = {//后，下，右，上，左
		-0.5f, -0.5f, -0.5f,//后5
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		 -0.5f, -0.5f, 0.5f,//4
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, 0.5f,

		 0.5f,  0.5f, 0.5f,//3
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, 0.5f,

		-0.5f,  0.5f, 0.5f,//3
		-0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, 0.5f,

		-0.5f, -0.5f, 0.5f//1
	};

	struct pointData {
		std::shared_ptr<VertexArray> pointVertexArray;
	};
	static pointData s_pointData;

	void DrawPoint::Bind() const
	{
		s_pointData.pointVertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> point_VertexBuffer;
		point_VertexBuffer.reset(VertexBuffer::Create(point_Vertices, sizeof(point_Vertices)));

		point_VertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
			}
		);

		s_pointData.pointVertexArray->AddVertexBuffer(point_VertexBuffer);

	}
	void DrawPoint::OnDraw(const std::shared_ptr<Shader>& shader) const
	{
		s_pointData.pointVertexArray->Bind();
		//Renderer::DrawLines(s_pointData.pointVertexArray);
		//glDrawElements(GL_LINE_STRIP, 24, GL_UNSIGNED_INT, nullptr);
		glDrawArrays(GL_LINE_STRIP, 0, 16);

		//glDrawArrays(GL_POINTS, 0, 3);
		s_pointData.pointVertexArray->Unbind();
	}
}