#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

#include "Mesh.h"
#include "Texture.h"
#include "Terrain.h"

class Node
{
protected:
	GLuint m_vao;
	Mesh& m_mesh;

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
	bool m_matrixShouldBeCalculated;
	glm::mat4 m_modelMatrix;
	Texture& m_texture;
public:

	Node(Mesh &mesh, Texture& texture);
	Node(Mesh &mesh, Texture& texture,
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	void setPosition(const glm::vec3& v);
	void setRotation(const glm::vec3& v);
	void setScale(const glm::vec3& v);

	const glm::vec3& getPosition() const;
	const glm::vec3& getRotation() const;
	const glm::vec3& getScale() const;

	glm::mat4 getModelMatrix() const;

	void move(const glm::vec3& v);
	void rotate(const glm::vec3& v);
	void scale(const glm::vec3& v);

	void calculateMatrices();
	void draw() const;
	~Node();
};

