#include "GUI.h"

float clamp(float x, float a, float b)
{
	return
		a < b ?
		x < a ? a :
		x > b ? b : x
		:
		x > a ? a :
		x < b ? b : x;
}


GUI::GUI()
{
}


GUI::~GUI()
{
}


billboard::billboard()
{
	GLfloat vertices[] =
	{
		0.0f, 0.5f,
		0.0f, 0.0f,
		0.5f, 0.5f,
		0.5f, 0.0f
	};

	glGenVertexArrays(1, &m_vao);
}

billboard::~billboard()
{
	glDeleteVertexArrays(1, &m_vao);
}

void billboard::draw()
{
	glBindVertexArray(m_vao);
	glUniform2f(glGetUniformLocation(ShaderManager::program2D, "offset"), m_position.x, m_position.y);
	glUniform2f(glGetUniformLocation(ShaderManager::program2D, "size"), m_size.x, m_size.y);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}