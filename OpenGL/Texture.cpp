#include "Texture.h"
#include <SOIL\SOIL.h>


std::map<char*, Texture*> Texture::textures;

Texture::Texture()
{

}

Texture::Texture(const char * path, GLuint wrap_s, GLuint wrap_t, GLuint min_filter, GLuint mag_filter)
{
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);

	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);

	if (wrap_s == GL_CLAMP_TO_BORDER)
	{
		GLfloat borderColor[] = { 1.0, 1.0, 1.0, 0.0 };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);



	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_texture);
}

void Texture::terminate()
{
	for (auto tex : textures)
		delete tex.second;
}
