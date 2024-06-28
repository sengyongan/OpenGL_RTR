#include "DrawGamma.h"
//平面
namespace Opengl {
	float gamma_Vertices[] = {//后，下，右，上，左
		// positions            // normals         // texcoords
		 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
		-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,

		 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
		 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f
	};

	struct gammaData {
		std::shared_ptr<VertexArray> gammaVertexArray;
	};
	static gammaData s_gammaData;

	void DrawGamma::Bind() const
	{
		s_gammaData.gammaVertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> gamma_VertexBuffer;
		gamma_VertexBuffer.reset(VertexBuffer::Create(gamma_Vertices, sizeof(gamma_Vertices)));

		gamma_VertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal" },
				{ ShaderDataType::Float2, "a_Texcoord" }
			}
		);

		s_gammaData.gammaVertexArray->AddVertexBuffer(gamma_VertexBuffer);

	}
	void DrawGamma::OnDraw(const std::shared_ptr<Shader>& shader) const
	{
		s_gammaData.gammaVertexArray->Bind();
		//Renderer::DrawLines(s_gammaData.gammaVertexArray);
		//glDrawElements(GL_LINE_STRIP, 24, GL_UNSIGNED_INT, nullptr);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawArrays(GL_POINTS, 0, 3);
		s_gammaData.gammaVertexArray->Unbind();
	}
}