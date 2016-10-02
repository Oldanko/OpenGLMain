#include "Mesh.h"



Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<float> &vertices, std::vector<unsigned int> &indices)
{
	m_indicesNum = indices.size();

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);
}

void Mesh::bindVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(float)));

}

void Mesh::bindEBO()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void Mesh::draw()
{
	glDrawElements(GL_TRIANGLES, m_indicesNum, GL_UNSIGNED_INT, 0);
}

GLuint Mesh::indicesNum()
{
	return m_indicesNum;
}





Mesh::~Mesh()
{
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}
