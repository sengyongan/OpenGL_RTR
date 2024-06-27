#include "DrawPlanet.h"
#include"Model.h"

namespace Opengl {

	struct planetData {
		std::shared_ptr<VertexArray> planetVertexArray;
		std::shared_ptr<Model> m_Rock;//大量的
		std::shared_ptr<Model> m_Planet;//唯一的
		unsigned int amount = 100000;
		glm::mat4* modelMatrices;
	};
	static planetData s_planetData;

	void DrawPlanet::Bind() const
	{
		//在model中已经VAO了
		s_planetData.m_Rock = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/rock/rock.obj");
		s_planetData.m_Planet = std::make_unique<Model>("D:/OpenGL_C++_Demo/OpenGl_Demo/OpenGl/resources/objects/planet/planet.obj");
		// generate a large list of semi-random model transformation matrices
		// ------------------------------------------------------------------
		s_planetData.modelMatrices = new glm::mat4[s_planetData.amount];
		srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
		float radius = 150.0;
		float offset = 25.0f;
		for (unsigned int i = 0; i < s_planetData.amount; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			// 1. translation: displace along circle with 'radius' in range [-offset, offset]
			float angle = (float)i / (float)s_planetData.amount * 360.0f;
			float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float x = sin(angle) * radius + displacement;
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float z = cos(angle) * radius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));

			// 2. scale: Scale between 0.05 and 0.25f
			float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
			model = glm::scale(model, glm::vec3(scale));

			// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
			float rotAngle = static_cast<float>((rand() % 360));
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

			// 4. now add to list of matrices
			s_planetData.modelMatrices[i] = model;
		}
		//m_Rock重设VAO
		// configure instanced array
		// -------------------------
		unsigned int buffer;//新建VBO，将modelMatrices存入buffer
		glGenBuffers(1, &buffer);//bind了有这个状态了，
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, s_planetData.amount * sizeof(glm::mat4), &s_planetData.modelMatrices[0], GL_STATIC_DRAW);

		// -----------------------------------------------------------------------------------------------------------------------------------
		for (unsigned int i = 0; i < s_planetData.m_Rock->meshes.size(); i++)//为周围的星球，设置VAO布局
		{
			unsigned int VAO = s_planetData.m_Rock->meshes[i].VAO;//变为全局变量，可以修改
			glBindVertexArray(VAO);
			// mat4-> 4 * vec4
			glEnableVertexAttribArray(3);//索引3开始
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);

			glBindVertexArray(0);
		}
	}
	void DrawPlanet::OnDraw(const std::shared_ptr<Shader>& shader) const//绘制rock
	{

	}
	void DrawPlanet::OnDrawPlanet(const std::shared_ptr<Shader>& shader) const
	{
		shader->Bind();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
		model = glm::translate(model, glm::vec3(15.0f, 0.0f, 0.0f));
		shader->SetMat4("model", model);
		s_planetData.m_Planet->Draw(shader);
		shader->Unbind();
	}
	void DrawPlanet::OnDrawRock(const std::shared_ptr<Shader>& shader) const
	{
		shader->Bind();
		shader->SetInt("texture_diffuse1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, s_planetData.m_Rock->textures_loaded[0].id);
		for (unsigned int i = 0; i < s_planetData.m_Rock->meshes.size(); i++)
		{
			glBindVertexArray(s_planetData.m_Rock->meshes[i].VAO);
			glDrawElementsInstanced(
				GL_TRIANGLES, s_planetData.m_Rock->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, s_planetData.amount
			);
			glBindVertexArray(0);
		}
		shader->Unbind();
	}
}