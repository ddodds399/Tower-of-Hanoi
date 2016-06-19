#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <string>
#include <GL/glew.h>

class Texture
{
public:
	Texture(const std::string& fileName);
	Texture(std::vector<const GLchar*> faces);
	void Bind();

	virtual ~Texture();
protected:
private:
	Texture(const Texture& other) {}
	void operator=(const Texture& other) {}

	GLuint m_texture;
	bool isCubemap = false;
};

#endif // TEXTURE_H