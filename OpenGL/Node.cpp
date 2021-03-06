#include "Node.h"

Node::Node(Mesh &mesh, Texture &texture) : m_mesh(mesh), m_texture(texture)
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	mesh.bindVBO();
	mesh.bindEBO();
	glBindVertexArray(0);
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_matrixShouldBeCalculated = true;
	calculateMatrices();
}

Node::Node(Mesh & mesh, Texture & texture, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : m_mesh(mesh), m_texture(texture)
{

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	mesh.bindVBO();
	mesh.bindEBO();
	glBindVertexArray(0);
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
	m_matrixShouldBeCalculated = true;
	calculateMatrices();
}

void Node::setPosition(const glm::vec3& v)
{
	m_position = v; 
	m_matrixShouldBeCalculated = true;
}

void Node::setRotation(const glm::vec3& v)
{
	m_rotation = v;
	m_matrixShouldBeCalculated = true;
}

void Node::setScale(const glm::vec3& v)
{
	m_scale = v;
	m_matrixShouldBeCalculated = true;
}

const glm::vec3& Node::getPosition() const
{
	return m_position;
}

const glm::vec3& Node::getRotation() const
{
	return m_rotation;
}

const glm::vec3& Node::getScale() const
{
	return m_scale;
}

glm::mat4 Node::getModelMatrix() const
{
	return m_modelMatrix;
}

void Node::move(const glm::vec3& v)
{
	m_position += v;
	m_matrixShouldBeCalculated = true;
}

void Node::rotate(const glm::vec3& v)
{
	m_rotation += v;
	m_matrixShouldBeCalculated = true;
}

void Node::scale(const glm::vec3& v)
{
	m_scale += v;
	m_matrixShouldBeCalculated = true;
}

void Node::calculateMatrices()
{
	if (m_matrixShouldBeCalculated)
		m_modelMatrix =

			glm::translate(m_position)
			*glm::rotate(m_rotation.x, glm::vec3(1, 0, 0))
			* glm::rotate(m_rotation.z, glm::vec3(0, 0, 1))
			* glm::rotate(m_rotation.y, glm::vec3(0, 1, 0))
			* glm::scale(m_scale);
}

void Node::draw() const
{
	m_texture.bind();
	glBindVertexArray(m_vao);
	m_mesh.draw();
}


Node::~Node()
{
	glDeleteVertexArrays(1, &m_vao);
}
