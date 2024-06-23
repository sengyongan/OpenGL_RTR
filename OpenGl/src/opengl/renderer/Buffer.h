#pragma once
//layout&&VBO,EBO

#include"hzpch.h"
#include "opengl/core/core.h"

namespace  Opengl {
	//自定义类型标签（float对应vec浮点向量）
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};
	//当前类型所需步长（总字节）
	static unsigned int ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return 4;
		case ShaderDataType::Float2:   return 4 * 2;//一个字节数 * 个数
		case ShaderDataType::Float3:   return 4 * 3;
		case ShaderDataType::Float4:   return 4 * 4;
		case ShaderDataType::Mat3:     return 4 * 3 * 3;
		case ShaderDataType::Mat4:     return 4 * 4 * 4;
		case ShaderDataType::Int:      return 4;
		case ShaderDataType::Int2:     return 4 * 2;
		case ShaderDataType::Int3:     return 4 * 3;
		case ShaderDataType::Int4:     return 4 * 4;
		case ShaderDataType::Bool:     return 1;
		}

		return 0;
	}
	//自定义layout的数据结构体
	struct BufferElement
	{
		//位置值Index（通过++自增）（参数1），类型，个数，……（参数4，是否正规化），总步长，相对偏移，6个参数
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		std::string Name;
		ShaderDataType Type;//（参数3）
		unsigned int Size;
		unsigned int Offset;
		bool Normalized;

		BufferElement() {}

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		unsigned int GetComponentCount() const//个数（参数2）
		{
			switch (Type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3 * 3;//个数
			case ShaderDataType::Mat4:    return 4 * 4;
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
			}

			return 0;
		}
	};
	//缓冲布局（对每个列表成员布局）
	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)//列表（每个成员结构体）
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();//初始化缓冲布局，就会计算步长
		}

		inline unsigned int GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:

		void CalculateOffsetsAndStride()//计算步长（总步长，偏移量）
		{
			unsigned int offset = 0;//（参数6）
			m_Stride = 0;//（参数5）
			for (auto& element : m_Elements)//循环每个值
			{
				element.Offset = offset;//当前element的步长
				offset += element.Size;
				m_Stride += element.Size;//计算总的步长
			}
		}
	private:
		std::vector<BufferElement> m_Elements;////列表
		unsigned int m_Stride = 0;//初始步长
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		//
		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		//
		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual unsigned int GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t size);
	};
}
