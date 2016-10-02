#pragma once
#include <GL\glew.h>

class Texture
{
	GLuint m_texture;
public:
	Texture();
	Texture(const char * path, GLuint wrap_s, GLuint wrap_t, GLuint min_filter, GLuint mag_filter);
	void bind();
	~Texture();
};

