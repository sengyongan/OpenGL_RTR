#include"DrawSphere.h"

namespace Opengl {

    const unsigned int X_SEGMENTS = 64;//segments
    const unsigned int Y_SEGMENTS = 64;
    const float PI = 3.14159265359f;

    struct SphereData {
        std::shared_ptr<VertexArray> SphereVertexArray;
        unsigned int indexCount;

        //顶点数据（位置，纹理坐标，法线）
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;
        //
        std::vector<float> data;
        //
        unsigned int sphereVAO = 0;
    };
    static SphereData s_SphereData;



	void DrawSphere::Bind() const
	{
        //
        //4096个顶点，绘制整个球体，按照经度半圈半圈，自上到下绘制，最后所有经度线都离散的绘制了
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)//纬度有64个顶点
        {
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)//每个经度半圈有64个顶点
            {
                float xSegment = (float)x / (float)X_SEGMENTS;//分隔绕y轴的角度
                float ySegment = (float)y / (float)Y_SEGMENTS;//分隔绕x或z的角度，

                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);//弧度制
                float yPos = std::cos(ySegment * PI);//
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);//

                s_SphereData.positions.push_back(glm::vec3(xPos, yPos, zPos));
                s_SphereData.uv.push_back(glm::vec2(xSegment, ySegment));
                s_SphereData.normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }
        //上面仅仅是绘制了平面的4个顶点，实则需要6个顶点，
        for (int i = 0; i < Y_SEGMENTS; i++)//第0个顶点和第65个顶点对齐，相邻的维度，绘制3角形
        {
            for (int j = 0; j < X_SEGMENTS; j++)
            {

                 s_SphereData.indices.push_back(i * (X_SEGMENTS + 1) + j);
                 s_SphereData.indices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
                 s_SphereData.indices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
                 
                 s_SphereData.indices.push_back(i * (X_SEGMENTS + 1) + j);
                 s_SphereData.indices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
                 s_SphereData.indices.push_back(i * (X_SEGMENTS + 1) + j + 1);
            }
        }

        s_SphereData.indexCount = static_cast<unsigned int>(s_SphereData.indices.size());
        //data将每个顶点的三个数据和为一个-》最终每个顶点的数据

        for (unsigned int i = 0; i < s_SphereData.positions.size(); ++i)
        {
             s_SphereData.data.push_back( s_SphereData.positions[i].x);
             s_SphereData.data.push_back( s_SphereData.positions[i].y);
             s_SphereData.data.push_back( s_SphereData.positions[i].z);
            if (s_SphereData.normals.size() > 0)
            {
                 s_SphereData.data.push_back( s_SphereData.normals[i].x);
                 s_SphereData.data.push_back( s_SphereData.normals[i].y);
                 s_SphereData.data.push_back( s_SphereData.normals[i].z);
            }
            if (s_SphereData.uv.size() > 0)
            {
                 s_SphereData.data.push_back( s_SphereData.uv[i].x);
                 s_SphereData.data.push_back( s_SphereData.uv[i].y);
            }
        }

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        
        glGenVertexArrays(1, &s_SphereData.sphereVAO);
        glBindVertexArray(s_SphereData.sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, s_SphereData.data.size() * sizeof(float), &s_SphereData.data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_SphereData.indices.size() * sizeof(unsigned int), &s_SphereData.indices[0], GL_STATIC_DRAW);
        unsigned int stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

	}
	void DrawSphere::OnDraw(const std::shared_ptr<Shader>& shader) const
	{
        glBindVertexArray(s_SphereData.sphereVAO);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 设置为线框模式

        //glDrawElements(GL_LINES, s_SphereData.indexCount, GL_UNSIGNED_INT, 0); //
        glDrawElements(GL_TRIANGLE_STRIP, s_SphereData.indexCount, GL_UNSIGNED_INT, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // 恢复到填充模式
	}
}
