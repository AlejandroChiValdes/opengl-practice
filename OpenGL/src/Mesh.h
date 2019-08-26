#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"
#include "Texture.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
public:
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<Texture> Textures;

	Mesh(std::vector<Vertex> v, std::vector<unsigned int> i, std::vector<Texture> t);
	void Draw(Shader& shader);

private:
	unsigned int VAO, VBO, EBO;

	void SetupMesh();
};