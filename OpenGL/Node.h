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
	Texture& m_texture;

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
	bool m_matrixShouldBeCalculated;
	glm::mat4 m_modelMatrix;
public:
	Node(Mesh &mesh, Texture& texture);
	Node(Mesh &mesh, Texture& texture,
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

	void setPosition(glm::vec3 v);
	void setRotation(glm::vec3 v);
	void setScale(glm::vec3 v);

	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();

	glm::mat4 getModelMatrix();

	void move(glm::vec3 v);
	void rotate(glm::vec3 v);
	void scale(glm::vec3 v);

	void calculateMatrices();
	void draw();
	~Node();
};

