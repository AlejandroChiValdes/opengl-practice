#include "Renderer.h"
#include <iostream>

void GLClearError()
{
	// cycle through
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogError(const char* function, const char* file, int line)
{
	bool success(true);
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error]: (" << error << ")" << "from function " << function << "in file " << file <<
			"on line " << line << std::endl;
		success = false;
	}
	return success;
}

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}