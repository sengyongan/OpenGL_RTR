#include "DrawGeometry.h"

namespace Opengl {
	float geometry_Vertices[] = {//ºó£¬ÏÂ£¬ÓÒ£¬ÉÏ£¬×ó
		//-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
		// 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
		// 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
		//-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left	};
		-0.5f, -0.5f, -0.5f,	 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	 1.0f, 1.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,	 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	 1.0f, 1.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,	 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,	 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,	 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	 1.0f, 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,	 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	 1.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,	 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	 0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,	 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	 1.0f, 1.0f, 0.0f,

		-0.5f,  0.5f, -0.5f,	 1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,	 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	 1.0f, 1.0f, 0.0f
	};
	struct geometryData {
		std::shared_ptr<VertexArray> geometryVertexArray;
	};
	static geometryData s_geometryData;

	void DrawGeometry::Bind() const
	{
		s_geometryData.geometryVertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> geometry_VertexBuffer;
		geometry_VertexBuffer.reset(VertexBuffer::Create(geometry_Vertices, sizeof(geometry_Vertices)));

		geometry_VertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Color" }
			}
		);

		s_geometryData.geometryVertexArray->AddVertexBuffer(geometry_VertexBuffer);

	}
	void DrawGeometry::OnDraw(const std::shared_ptr<Shader>& shader) const
	{
		s_geometryData.geometryVertexArray->Bind();
		glDrawArrays(GL_POINTS, 0, 24);

		s_geometryData.geometryVertexArray->Unbind();
	}
}