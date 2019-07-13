#pragma once
#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	// width and height of image, in addition to the image's bytes per pixel
	int m_Width, m_Height, m_BPP;

public:
	Texture(const std::string& path);
	~Texture();

	// OpenGL allows you to bind multiple textures at once. The number of texture 'slots' you have depends on your
	// GPU's implementation of OpenGL
	void Bind(unsigned int slot = 0) const;

	void Unbind() const;
};