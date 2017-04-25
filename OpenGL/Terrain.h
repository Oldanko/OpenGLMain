#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include "Texture.h"

class Terrain
{
	unsigned int m_size;
	float * m_heightmap;
	GLuint m_vao, m_ebo;
	GLuint m_vbo[2];

	float * getGrid();
	unsigned int * indexicate();
public:
	Texture &noise;
	Terrain();
	Terrain(const char * heightmap);
	~Terrain();
	bool loadHeightMap(const char * path);
	void draw() const;

	float findHeight(const glm::vec2& position) const;
	const glm::vec2& findSlope(const glm::vec2& position) const;
	
	unsigned int size() const { return m_size; }
};

