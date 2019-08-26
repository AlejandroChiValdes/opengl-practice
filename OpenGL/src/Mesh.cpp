#include "Mesh.h"
#include <GL/glew.h>
#include "Renderer.h"
#include <iostream>
Mesh::Mesh(std::vector<Vertex> v, std::vector<unsigned int> i, std::vector<Texture> t)
	:Vertices(v), Indices(i), Textures(t)
{
	SetupMesh();
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	// Bind each of our mesh's textures to uniforms in our shader and use
	// them to draw our mesh
	for (unsigned int i = 0; i < Textures.size(); ++i)
	{
		// Activate a new texture unit that we will bind our texture object to
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = Textures[i].type;

		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
		}
		else if (name == "texture_specular")
		{
			number = std::to_string(specularNr++);
		}

		shader.SetUniform1i(name + number, i);
		GLCall(glBindTexture(GL_TEXTURE_2D, Textures[i].id));
	}
	// Reset our active texture back to the default
	glActiveTexture(GL_TEXTURE0);

	// Bind our vertex array object, which contains our vertex buffer object 
	// that was bound to our VAO during our SetupMesh fn call
	glBindVertexArray(VAO);
	// Draw the vertices specified by our index buffer object
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}
void Mesh::SetupMesh()
{
	// Generate buffer objects for our Vertex Array, Vertex Buffer and Index (Element) Buffers.
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &VBO));
	GLCall(glGenBuffers(1, &EBO));

	GLCall(glBindVertexArray(VAO));
	// Bind our vertex data to the active vertex buffer object
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	// Specify the layout of our vertex data and bind our Vertex data to our VBO.
	GLCall(glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW));
	// Bind our index data to the active element array object
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW));

	// Binds our Vertex Buffer Layout to our Vertex Array
	// Position Data
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
	// Normal Data
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));
	// Texture Coords Data
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));

}