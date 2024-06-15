#pragma once
//VAO
#include"hzpch.h"
#include "opengl/core/core.h"
#include"Buffer.h"

namespace  Opengl {
	class  VertexArray
	{
	public:
		VertexArray();
		~VertexArray();
		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
	private:
		unsigned int m_RendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;//VAO对应绑定的缓冲（因为可以绑定不同的缓冲）-》从而获得所绑定的顶点属性
		std::shared_ptr<IndexBuffer> m_IndexBuffer;

	};

}
