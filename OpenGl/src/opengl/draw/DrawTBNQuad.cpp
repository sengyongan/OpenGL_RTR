#include "DrawTBNQuad.h"

namespace Opengl {

    float tbnFloats[216];//TBN -> 36个顶点 * 6(TBN)
    float finishVertices[504];//合并 ->36* (8 + 6)

    float tbnvertices[] = {//CUBE -> position,normal,Texcoords  -> 36个顶点

        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,

        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,

        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,

        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,

        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f

    };

	struct TBNData {
		std::shared_ptr<VertexArray> TBNVertexArray;
	};
	static TBNData s_TBNData;

	void DrawTBNQuad::Bind() const
	{
        ///计算TBN保存到tbnFloats数组，6个面，每个面2个三角形，计算每个三角形的TBN
        for (int i = 0; i < 12; ++i)

        {

            glm::vec3 tbn;

            int firstIndex = i * 24; // 三角形第 1 个顶点坐标起始索引

            int secondIndex = firstIndex + 8; // 三角形第 2 个顶点坐标起始索引

            int thirdIndex = secondIndex + 8; // 三角形第 3 个顶点坐标起始索引

            //获取当前三角形的 3个顶点坐标，和3个纹理坐标

            glm::vec3 pos1(tbnvertices[firstIndex], tbnvertices[firstIndex + 1], tbnvertices[firstIndex + 2]);

            glm::vec3 pos2(tbnvertices[secondIndex], tbnvertices[secondIndex + 1], tbnvertices[secondIndex + 2]);

            glm::vec3 pos3(tbnvertices[thirdIndex], tbnvertices[thirdIndex + 1], tbnvertices[thirdIndex + 2]);


            glm::vec2 uv1(tbnvertices[firstIndex + 6], tbnvertices[firstIndex + 7]);

            glm::vec2 uv2(tbnvertices[secondIndex + 6], tbnvertices[secondIndex + 7]);

            glm::vec2 uv3(tbnvertices[thirdIndex + 6], tbnvertices[thirdIndex + 7]);


            glm::vec3 edge1 = pos2 - pos1;

            glm::vec3 edge2 = pos3 - pos1;

            glm::vec2 deltaUV1 = uv2 - uv1;

            glm::vec2 deltaUV2 = uv3 - uv1;

            //计算TBN（三角形每个顶点的TBN都是一样的）

            glm::vec3 tangent;

            glm::vec3 bitTangent;

            GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);

            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);

            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

            tangent = glm::normalize(tangent);

            bitTangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);

            bitTangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);

            bitTangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

            bitTangent = glm::normalize(bitTangent);

            //添加当前三角形
            //3个顶点 * 6（TBN） = 18

            int startTBNIndex = i * 18;

            tbnFloats[startTBNIndex + 0] = tangent.x;

            tbnFloats[startTBNIndex + 1] = tangent.y;

            tbnFloats[startTBNIndex + 2] = tangent.z;

            tbnFloats[startTBNIndex + 3] = bitTangent.x;

            tbnFloats[startTBNIndex + 4] = bitTangent.y;

            tbnFloats[startTBNIndex + 5] = bitTangent.z;

            tbnFloats[startTBNIndex + 6] = tangent.x;

            tbnFloats[startTBNIndex + 7] = tangent.y;

            tbnFloats[startTBNIndex + 8] = tangent.z;

            tbnFloats[startTBNIndex + 9] = bitTangent.x;

            tbnFloats[startTBNIndex + 10] = bitTangent.y;

            tbnFloats[startTBNIndex + 11] = bitTangent.z;

            tbnFloats[startTBNIndex + 12] = tangent.x;

            tbnFloats[startTBNIndex + 13] = tangent.y;

            tbnFloats[startTBNIndex + 14] = tangent.z;

            tbnFloats[startTBNIndex + 15] = bitTangent.x;

            tbnFloats[startTBNIndex + 16] = bitTangent.y;

            tbnFloats[startTBNIndex + 17] = bitTangent.z;

        }
        ///合并3个数组，36个顶点
        for (int i = 0; i < 36; ++i)

        {

            int finishStartIndex = i * 14;

            int verticesStartIndex = i * 8;

            int tbnStartIndex = i * 6;

            finishVertices[finishStartIndex + 0] = tbnvertices[verticesStartIndex + 0];

            finishVertices[finishStartIndex + 1] = tbnvertices[verticesStartIndex + 1];

            finishVertices[finishStartIndex + 2] = tbnvertices[verticesStartIndex + 2];

            finishVertices[finishStartIndex + 3] = tbnvertices[verticesStartIndex + 3];

            finishVertices[finishStartIndex + 4] = tbnvertices[verticesStartIndex + 4];

            finishVertices[finishStartIndex + 5] = tbnvertices[verticesStartIndex + 5];

            finishVertices[finishStartIndex + 6] = tbnvertices[verticesStartIndex + 6];

            finishVertices[finishStartIndex + 7] = tbnvertices[verticesStartIndex + 7];

            finishVertices[finishStartIndex + 8] = tbnFloats[tbnStartIndex + 0];

            finishVertices[finishStartIndex + 9] = tbnFloats[tbnStartIndex + 1];

            finishVertices[finishStartIndex + 10] = tbnFloats[tbnStartIndex + 2];

            finishVertices[finishStartIndex + 11] = tbnFloats[tbnStartIndex + 3];

            finishVertices[finishStartIndex + 12] = tbnFloats[tbnStartIndex + 4];

            finishVertices[finishStartIndex + 13] = tbnFloats[tbnStartIndex + 5];

        }

        ///添加到VAO//////////////////////////////////////////////////////////////////////////////


        //
		s_TBNData.TBNVertexArray = std::make_unique<VertexArray>();

		std::shared_ptr<VertexBuffer> TBN_VertexBuffer;
		TBN_VertexBuffer.reset(VertexBuffer::Create(finishVertices, sizeof(finishVertices)));

		TBN_VertexBuffer->SetLayout(
			{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal" },
				{ ShaderDataType::Float2, "a_Texcoords" },
				{ ShaderDataType::Float3, "a_Tangent" },
				{ ShaderDataType::Float3, "a_Bitangent" }
			}
		);

		s_TBNData.TBNVertexArray->AddVertexBuffer(TBN_VertexBuffer);
	}
	void DrawTBNQuad::OnDraw(const std::shared_ptr<Shader>& shader) const
	{
		s_TBNData.TBNVertexArray->Bind();
		glDrawArrays(GL_TRIANGLES, 0,36);
		s_TBNData.TBNVertexArray->Unbind();

	}
}