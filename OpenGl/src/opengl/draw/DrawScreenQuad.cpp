#include "DrawScreenQuad.h"

namespace Opengl {
	//quad
	float ScreenVertices[] = {//position + texture左上，左下，右下，右上
		// positions        // texture Coords
		   -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		   -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	//quad
	//unsigned int screen_Indices[6] = { 0, 1, 2, 2, 3, 0 };


	struct ScreenData {
		std::shared_ptr<VertexArray> ScreenVertexArray;
	};
	static ScreenData s_ScreenData;

	void DrawScreenQuad::Bind() const
	{
		s_ScreenData.ScreenVertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> point_VertexBuffer;
		point_VertexBuffer.reset(VertexBuffer::Create(ScreenVertices, sizeof(ScreenVertices)));

		point_VertexBuffer->SetLayout(
			{
					{ ShaderDataType::Float3, "a_Position" },
					{ ShaderDataType::Float2, "a_TexCoord" }
			}
		);

		s_ScreenData.ScreenVertexArray->AddVertexBuffer(point_VertexBuffer);

		//std::shared_ptr<IndexBuffer> point_IndexBuffer;
		//point_IndexBuffer.reset(IndexBuffer::Create(screen_Indices, sizeof(screen_Indices) / sizeof(unsigned int)));

		//s_ScreenData.ScreenVertexArray->SetIndexBuffer(point_IndexBuffer);

	}
	void DrawScreenQuad::OnDraw(const std::shared_ptr<Shader>& shader) const
	{
		s_ScreenData.ScreenVertexArray->Bind();
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		s_ScreenData.ScreenVertexArray->Unbind();
	}
}