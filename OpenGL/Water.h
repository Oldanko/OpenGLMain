#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "Camera.h"

class Water
{
	glm::vec2 m_A, m_B;
	GLuint m_vao,m_ebo;
	GLuint m_vbo;
	float m_height;
public:
	glm::mat4 m_reflectionMatrix;
	GLuint m_fbo, m_reflectionTex, m_reflectionRbo;
	Water();
	Water(glm::vec2 A, glm::vec2 B, float height);
	void PrepareVertexArray();
	void PrepareFrameBuffer();
	void bindFrameBuffer();
	void bindReflectionTexture();
	void calculateMatrices(Camera & camera);
	float height();
	void draw();	
	~Water();
};

