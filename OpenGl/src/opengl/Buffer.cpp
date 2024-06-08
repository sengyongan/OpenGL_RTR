#include "Buffer.h"

#include"Buffer_Abstraction.h"

namespace Opengl {
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		return new OpenGLVertexBuffer(vertices, size);
	}
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		return new OpenGLIndexBuffer(indices, size);
	}
}