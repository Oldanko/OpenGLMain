#include "Texture.h"
#include <SOIL\SOIL.h>
#include <iostream>

enum {
	NEAREST = 0,
	LINEAR
};
enum
{
	NEAREST_MIPMAP_NEAREST = 0, 
	LINEAR_MIPMAP_NEAREST, 
	NEAREST_MIPMAP_LINEAR,
	LINEAR_MIPMAP_LINEAR
};
enum 
{
	REPEAT = 0,
	MIRRORED_REPEAT,
	CLAMP_TO_EDGE,
	CLAMP_TO_BORDER
};


std::map<std::string, Texture*> Texture::textures;

void Texture::init()
{
	char name[32];
	char path[64];

	GLuint wrap_s = 0;
	GLuint wrap_t = 0;
	GLuint min_filter = 0;
	GLuint mag_filter = 0;

	FILE * file = fopen("resources/Textures/Textures.ls", "r");
	if (file == NULL)
	{
		std::cout << "Couldn't open Textures.ls";
		return;
	}

	while (!feof(file))
	{
		fscanf(file, "%s %s %i %i %i %i\n", name, path, &wrap_s, &wrap_t, &min_filter, &mag_filter);

		switch (wrap_s)
		{
		case REPEAT:			wrap_s = GL_REPEAT;
			break;
		case MIRRORED_REPEAT:	wrap_s = GL_MIRRORED_REPEAT;
			break;
		case CLAMP_TO_EDGE:		wrap_s = GL_CLAMP_TO_EDGE;
			break;
		case CLAMP_TO_BORDER:	wrap_s = GL_CLAMP_TO_BORDER;
			break;
		}

		switch (wrap_t)
		{
		case REPEAT:			wrap_t = GL_REPEAT;
			break;
		case MIRRORED_REPEAT:	wrap_t = GL_MIRRORED_REPEAT;
			break;
		case CLAMP_TO_EDGE:		wrap_t = GL_CLAMP_TO_EDGE;
			break;
		case CLAMP_TO_BORDER:	wrap_t = GL_CLAMP_TO_BORDER;
			break;
		}

		switch (min_filter)
		{
		case NEAREST_MIPMAP_NEAREST:	min_filter = GL_NEAREST_MIPMAP_NEAREST;
			break;
		case LINEAR_MIPMAP_NEAREST:		min_filter = GL_LINEAR_MIPMAP_NEAREST;
			break;
		case NEAREST_MIPMAP_LINEAR:		min_filter = GL_NEAREST_MIPMAP_LINEAR;
			break;
		case LINEAR_MIPMAP_LINEAR:		min_filter = GL_LINEAR_MIPMAP_LINEAR;
			break;
		}

		switch (mag_filter)
		{
		case NEAREST:			mag_filter = GL_NEAREST;
			break;
		case LINEAR:			mag_filter = GL_LINEAR;
			break;
		}
		textures[name] = new Texture(path, wrap_s, wrap_t, min_filter, mag_filter);

		std::cout << textures[name]->m_texture << "\n";
	}
	fclose(file);
}

Texture::Texture()
{

}

Texture::Texture(GLuint texture)
{
	m_texture = texture;
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
