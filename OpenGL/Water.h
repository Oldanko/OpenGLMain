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
	GLuint m_fbo[2], m_reflectionTex[3], m_reflectionRbo, m_refractionTex[3], m_refractionRbo;
	Water();
	Water(glm::vec2 A, glm::vec2 B, float height);
	void PrepareVertexArray();
	void PrepareFrameBuffer();
	void bindReflectionTexture();
	void bindRefractionTexture();
	void calculateMatrices(Camera & camera);
	float height();
	void draw();	
	void setHeight(float height) { m_height = height; }
	~Water();
};

