#include <glad/glad.h>
#include "Uniform.h"

namespace Opengl {
	Uniform::Uniform(unsigned int size, unsigned int binding)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW); 
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		//绑定到绑定点
		glBindBufferRange(GL_UNIFORM_BUFFER, binding, m_RendererID,binding,size);

	}
	Uniform::~Uniform()
	{
		glDeleteBuffers(1, &m_RendererID);

	}
	void Uniform::Bind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
	}
	void Uniform::Unbind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	void Uniform::SetData(const void* data, unsigned int size, unsigned int offset)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

	}
}