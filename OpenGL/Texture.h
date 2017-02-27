#pragma once
#include <GL\glew.h>
#include <map>

class Texture
{
	GLuint m_texture;
public:
	static std::map<std::string, Texture*> textures;
	static void init();
	static void terminate();

	Texture(); 
	Texture(GLuint texture);
	Texture(const char * path, GLuint wrap_s, GLuint wrap_t, GLuint min_filter, GLuint mag_filter);
	~Texture();

	void bind();
};