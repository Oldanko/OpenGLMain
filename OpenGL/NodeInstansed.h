#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>

#include "Mesh.h"
#include "Texture.h"

class NodeInstansed
{
	GLuint vao, m_matrixBuffer, m_num;
	Mesh &m_mesh;
	Texture &m_texture;
public:
	NodeInstansed(Mesh &mesh, Texture &texture, std::vector<glm::mat4> &matrices);
	void draw() const;
	~NodeInstansed();
};