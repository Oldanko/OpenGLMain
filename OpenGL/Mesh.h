#pragma once
#include <GL/glew.h>
#include <vector>

class Mesh
{
	GLuint vbo, ebo, m_indicesNum;
public:
	Mesh();
	Mesh(std::vector<float> &vertices, std::vector<unsigned int> &indices);
	void bindVBO();
	void bindEBO();
	void draw();
	GLuint indicesNum();
	~Mesh();
};

