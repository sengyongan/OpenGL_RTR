#pragma once
//uniform»º³å

namespace Opengl {
	class Uniform {
	public:
		Uniform(unsigned int size, unsigned int binding);
		~Uniform();

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, unsigned int size, unsigned int offset);
	private:
		unsigned int m_RendererID = 0;
	};
}