#pragma once
#include <GL/glew.h>
#include <vector>

class Mesh
{
	GLuint vbo, ebo, m_indicesNum;
public:
	Mesh();
	Mesh(std::vector<float> &vertices, std::vector<unsigned int> &indices);
	void bindVBO() const;
	void bindEBO() const;
	void draw() const;
	GLuint indicesNum() const;
	~Mesh();
};

