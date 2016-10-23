#include "Water.h"
#include <iostream>


Water::Water()
{
	float vertices[] =
	{
		0.0, 1.0, 0.0,
		100.0, 1.0, 0.0,
		100.0, 1.0, 100.0,
		0.0, 1.0, 100.0
	};

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);

	//delete[] vertices;

	unsigned int indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	glBindVertexArray(0);
}

Water::Water(glm::vec2 A, glm::vec2 B, float height)
{
	m_A = A;
	m_B = B;
	m_height = height;

	PrepareVertexArray();
	PrepareFrameBuffer();
}

void Water::PrepareVertexArray()
{
	float vertices[] =
	{
		m_A.x, m_height, m_A.y, 0.0, 0.0,
		m_A.x, m_height, m_B.y, 0.0, 1.0,
		m_B.x, m_height, m_B.y, 1.0, 1.0,
		m_B.x, m_height, m_A.y, 1.0, 0.0
	};

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), vertices, GL_STATIC_DRAW);

	unsigned int indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3*sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	glBindVertexArray(0);
}

void Water::PrepareFrameBuffer()
{
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(1, &m_reflectionTex);
	glBindTexture(GL_TEXTURE_2D, m_reflectionTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_reflectionTex, 0);

	glGenRenderbuffers(1, &m_reflectionRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_reflectionRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_reflectionRbo); 
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Water::bindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void Water::bindReflectionTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_reflectionTex);
}

void Water::calculateMatrices(Camera & camera)
{
	glm::vec3 target = camera.position();
	glm::vec3 pointOfView = camera.cameraGlobalPosition();

	m_reflectionMatrix = glm::lookAt(
		glm::vec3(pointOfView.x, m_height - pointOfView.y, pointOfView.z),
		glm::vec3(target.x, m_height - target.y, target.z),
		glm::vec3(0, -1, 0)
		);
}

float Water::height()
{
	return m_height;
}

void Water::draw()
{
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Water::~Water()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteFramebuffers(1, &m_fbo);
	glDeleteTextures(1, &m_reflectionTex);
	glDeleteRenderbuffers(1, &m_reflectionRbo);
}