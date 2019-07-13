#pragma once
#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

struct LayoutElement
{
	// Holds all of the parameters needed to make a glVertexAttribPointer call,
	// which will bind our current buffer to the VertexArray and specify a layout for
	// our buffer
	unsigned int type;
	unsigned int count;
	unsigned int normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:
			return 4;
		case GL_UNSIGNED_INT:
			return 4;
		case GL_UNSIGNED_BYTE:
			return 1;
		}
		ASSERT(false);
		return 0;
	}
};
class VertexBufferLayout
{
private:
	std::vector<LayoutElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		: m_Stride(0)
	{}
	

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += LayoutElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += LayoutElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		// why are char elements normalize while the other types aren't?
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += LayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<LayoutElement>& GetElements() const { return m_Elements; }

	inline unsigned int GetStride() const { return m_Stride; }
};