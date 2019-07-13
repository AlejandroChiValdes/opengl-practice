#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}
void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(VertexBuffer& vb, VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const std::vector<LayoutElement>& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const LayoutElement& element = elements[i];
		// enables attribute 0 for our currently bound vertex array
		GLCall(glEnableVertexAttribArray(i));
		// specifies attribute 0 for our currently bound vertex array
		// this line of code is what binds our vertex buffer to our vertex array, because we specify
		// that we want our currently bound buffer to be used to render the current object
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * LayoutElement::GetSizeOfType(element.type);
	}

}
