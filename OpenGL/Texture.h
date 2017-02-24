#pragma once
#include <GL\glew.h>
#include <map>

class Texture
{
	GLuint m_texture;
public:
	static std::map<char*, Texture*> textures;
	Texture();
	Texture(const char * path, GLuint wrap_s, GLuint wrap_t, GLuint min_filter, GLuint mag_filter);
	void bind();
	~Texture();
	static void terminate();
};