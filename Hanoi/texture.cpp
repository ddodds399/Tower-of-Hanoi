#include "texture.h"
#include "stb_image.h"
#include <iostream>
#include <fstream>
#include <cassert>

//Constructor for 2D textures
//Uses stb image loader and a file path.
Texture::Texture(const std::string & fileName)
{
	int width, height, numComponents;

	unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &numComponents, 4);

	if (imageData == NULL)
		std::cerr << "Unable to load texture: " << fileName << std::endl;

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
}

//Constructor for cubemap textures
Texture::Texture(std::vector<const GLchar*> faces)
{
	//Loads a cubemap texture from 6 individual texture faces

	isCubemap = true;

	glGenTextures(1, &m_texture);

	int width, height, numComponents;
	unsigned char* imageData;

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		imageData = stbi_load(faces[i], &width, &height, &numComponents, 3);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		stbi_image_free(imageData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

//Deconstructor
Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
}

//Used to bind a texture.
//Used before a mesh is drawn
void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0);
	//Checks type of texture
	if (isCubemap) 
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}
}