#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>

class Terrain
{
	unsigned int m_size;
	float * m_heightmap;
	GLuint m_vao, m_ebo;
	GLuint m_vbo[2];
	float * getGrid();
	unsigned int * indexicate();
public:
	Terrain();
	Terrain(const char * heightmap);
	~Terrain();
	bool loadHeightMap(const char * path);
	void draw();
	float findHeight(glm::vec2 position);
	glm::vec2 findSlope(glm::vec2 position);
	unsigned int size() { return m_size; }
};

