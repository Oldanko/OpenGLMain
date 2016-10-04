#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "Camera.h"

class Water
{
	glm::mat4 m_reflectionMatrix;
	glm::vec2 m_A, m_B;
	GLuint m_vao,m_ebo;
	GLuint m_vbo;
	GLuint m_fbo, m_reflectionTex, m_reflectionRbo;
	float m_height;
public:
	Water();
	Water(glm::vec2 A, glm::vec2 B, float height);
	void PrepareVertexArray();
	void PrepareFrameBuffer();
	void bindFrameBuffer();
	void bindReflectionTexture();
	void calculateMatrices(Camera & camera);
	void draw();	
	~Water();
};

