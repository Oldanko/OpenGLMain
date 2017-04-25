#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include "ShaderManager.h"
#include "Controls.h"
#include "WindowManager.h"

float clamp(float x, float a, float b);

class button
{
	glm::vec3 m_color;
	glm::vec2 m_size;
	glm::vec2 m_position;
	GLuint m_vao, m_vbo;

public:

	button() {}
	button(glm::vec2 size, glm::vec2 position)
	{
		m_size = size;
		m_position = position;

		GLfloat vertices[] =
		{
			0.0f, 0.0f,
			0.0f, m_size.y,
			m_size.x, m_size.y,

			0.0f, 0.0f,
			m_size.x, m_size.y,
			m_size.x, 0.0f
		};

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

		glBindVertexArray(0);

		m_color = glm::vec3(0.5f);
	}

	~button()
	{
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
	}

	void draw()
	{
		glBindVertexArray(m_vao);
		glUniform2f(glGetUniformLocation(ShaderManager::program2D, "offset"), m_position.x, m_position.y);
		glUniform3f(glGetUniformLocation(ShaderManager::program2D, "color"), m_color.x, m_color.y, m_color.z);
		glDrawArrays(GL_TRIANGLES, 0, 12);
	}

	void update()
	{
		glm::vec2 mouse = glm::vec2(Controls::x() / WindowManager::width() * 2 - 1,
			1 - Controls::y() / WindowManager::height() * 2);

		mouse.x > m_position.x && mouse.y > m_position.y &&
			mouse.x < m_position.x + m_size.x && mouse.y < m_position.y + m_size.y
			?
			m_color = glm::vec3(0.0, 1.0, 0.0)
			:
			m_color = glm::vec3(1.0, 0.0, 0.0);
	}
};

class slider
{
	glm::vec3 m_color;
	glm::vec2 m_position;
	GLfloat m_size;
	GLfloat m_sliderPosition;

	GLuint m_vao[2], m_vbo[2];

	bool m_isPressed;

public:

	slider() {}
	slider(glm::vec2 position, GLfloat size)
	{
		m_color = glm::vec3(1.0);
		m_sliderPosition = 0;
		m_isPressed = false;

		m_size = size;
		m_position = position;

		GLfloat vertices[] =
		{
			0.0f, 0.0f,
			0.0f, 0.01f,
			m_size, 0.01f,

			0.0f, 0.0f,
			m_size, 0.01f,
			m_size, 0.0f
		};

		glGenVertexArrays(2, m_vao);
		glGenBuffers(2, m_vbo);
		glBindVertexArray(m_vao[0]);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

		glBindVertexArray(0);

		GLfloat vertices2[] =
		{
			0.0f, 0.0f,
			-0.01f, 0.04f,
			0.01f, 0.04f,

		};

		glBindVertexArray(m_vao[1]);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), vertices2, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

		glBindVertexArray(0);

	}
	~slider()
	{
		glDeleteVertexArrays(2, m_vao);
		glDeleteBuffers(2, m_vbo);
	}
	void draw()
	{
		glBindVertexArray(m_vao[0]);
		glUniform2f(glGetUniformLocation(ShaderManager::program2D, "offset"), m_position.x, m_position.y);
		glUniform3f(glGetUniformLocation(ShaderManager::program2D, "color"), m_color.x, m_color.y, m_color.z);
		glDrawArrays(GL_TRIANGLES, 0, 12);

		glBindVertexArray(m_vao[1]);
		glUniform2f(glGetUniformLocation(ShaderManager::program2D, "offset"), m_position.x + m_sliderPosition, m_position.y);
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}

	void update()
	{
		/*glm::vec2 mouse = glm::vec2(Controls::x() / WindowManager::width() * 2 - 1,
			1 - Controls::y() / WindowManager::height() * 2);

		if (mouse.x > m_position.x && mouse.y > m_position.y &&
			mouse.x < m_position.x + m_size && mouse.y < m_position.y + 0.01 &&
			Controls::mouseInputs() & LMB_PRESS)
			m_isPressed = true;
		if (Controls::mouseInputs() & LMB_RELEASE)
			m_isPressed = false;
			
		m_isPressed ?
			m_sliderPosition = 
				clamp(mouse.x, m_position.x + m_size, m_position.x) - m_position.x
			:
			0;

		Controls::mouseInputs();*/
	}

	GLfloat value()
	{
		return m_sliderPosition / m_size;
	}
	void setValue(float value)
	{
		m_sliderPosition = clamp(value*m_size, 0, m_size);
	}
};

class billboard
{
public:
	glm::vec2 m_position;
	glm::vec2 m_size = glm::vec2(0.5);
	GLuint m_vao;

	billboard();
	~billboard();
	void draw();

};

class GUI
{
public:
	GUI();
	~GUI();
};

