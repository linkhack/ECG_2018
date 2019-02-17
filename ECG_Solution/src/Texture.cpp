#include "Texture.h"
#include "Utils.h"


Texture::Texture(std::string path)
{
	DDSImage img = loadDDS(path.c_str());
	if (img.size == 0) {
		std::cout << "Couldn't loade image file " << path << std::endl;
	}else {
		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, img.format, img.width, img.height, 0, img.size, img.image);
		glGenerateMipmap(GL_TEXTURE_2D);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &handle);
	std::cout << "Texture deleted" << std::endl;
}

void Texture::activateTexture(int unit)
{
	glBindTexture(GL_TEXTURE_2D, handle);
	glActiveTexture(GL_TEXTURE0 + unit);

}

