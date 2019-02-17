#pragma once
#include <string>
#include <Gl/glew.h>
class Texture
{
private:
	GLuint handle;
public:
	Texture(std::string path);
	~Texture();

	void activateTexture(int unit);
};

