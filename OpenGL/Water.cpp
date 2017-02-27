#include "Water.h"
#include "WindowManager.h"
#include "ShaderManager.h"
#include <iostream>
#include <glm\gtx\transform.hpp>


glm::vec3 intersection(glm::vec3 p1, glm::vec3 v1, glm::vec3 p2, glm::vec3 v2)
{
	float k = (v2.y*(p2.x - p1.x) + v2.x*(p1.y - p2.y)) / (v1.x*v2.y - v1.y*v2.x);

	return glm::vec3(
		p1.x + k*v1.x,
		p1.y + k*v1.y,
		p1.z + k*v1.z);
}

glm::vec3 refraction(glm::vec3 v, float n) //
{
	glm::vec3 vN = glm::normalize(v);
	float base = glm::length(glm::vec2(vN.x, vN.z));

	char sign = v.y > 0 ? -1 : 1;

	return
		glm::vec3(
			base*n / base * vN.x,
			sign * sqrt(1 - base*base),
			base*n / base * vN.z
		);
}

Water::Water()
{
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
		m_A.x, m_A.y, 0.0, 0.0,
		m_A.x, m_B.y, 0.0, 1.0,
		m_B.x, m_B.y, 1.0, 1.0,
		m_B.x, m_A.y, 1.0, 0.0
	};

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices, GL_STATIC_DRAW);

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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2*sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	glBindVertexArray(0);
}

void Water::PrepareFrameBuffer()
{
	glGenFramebuffers(2, m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo[0]);

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


	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo[1]);

	glGenTextures(1, &m_refractionTex);
	glBindTexture(GL_TEXTURE_2D, m_refractionTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_refractionTex, 0);

	glGenRenderbuffers(1, &m_refractionRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_refractionRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_refractionRbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Water::bindReflectionTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_reflectionTex);
}

void Water::bindRefractionTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_refractionTex);
}

void Water::calculateMatrices(Camera & camera)
{
	glm::vec3 target = camera.position();
	glm::vec3 POV = camera.cameraGlobalPosition();
	float rotation = camera.angles().y;

	m_reflectionMatrix = WindowManager::projectionMatrix() * glm::lookAt(
		glm::vec3(POV.x, m_height - POV.y, POV.z),
		glm::vec3(target.x, m_height - target.y, target.z),
		glm::vec3(0, -1, 0)
		);

	m_refractionMatrix = WindowManager::projectionMatrix() * camera.ViewMatrix();
}

float Water::height()
{
	return m_height;
}

void Water::draw()
{
	//glUniform1f(glGetUniformLocation(ShaderManager::programWater, "height"), m_height);
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Water::~Water()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteFramebuffers(2, m_fbo);
	glDeleteTextures(1, &m_reflectionTex);
	glDeleteRenderbuffers(1, &m_reflectionRbo);
	glDeleteTextures(1, &m_refractionTex);
	glDeleteRenderbuffers(1, &m_refractionRbo);

}