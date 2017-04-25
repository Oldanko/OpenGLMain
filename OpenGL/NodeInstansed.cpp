#include "NodeInstansed.h"

NodeInstansed::NodeInstansed(Mesh & mesh, Texture & texture, std::vector<glm::mat4> &matrices) : m_mesh(mesh), m_texture(texture)
{
	m_num = matrices.size();

	glGenBuffers(1, &m_matrixBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_matrixBuffer);
	glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(glm::mat4), &matrices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	m_mesh.bindVBO();
	m_mesh.bindEBO();

	GLsizei vec4Size = sizeof(glm::vec4);
	glBindBuffer(GL_ARRAY_BUFFER, m_matrixBuffer);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}

void NodeInstansed::draw() const
{
	m_texture.bind();
	glBindVertexArray(vao);
	glDrawElementsInstanced(GL_TRIANGLES, m_mesh.indicesNum(), GL_UNSIGNED_INT, 0, m_num);
}

//	GLuint vao, m_matrixBuffer, m_num;

NodeInstansed::~NodeInstansed()
{
	glDeleteBuffers(1, &m_matrixBuffer);
	glDeleteVertexArrays(1, &vao);
}