#include "Terrain.h"
#include <fstream>
#include <iostream>
#include "ShaderManager.h"

float linearInterpolaton(float a, float b, float x)
{
	return a*(1 - x) + b*x;
}

Terrain::Terrain() : noise(*Texture::textures["noise"])
{
	m_heightmap = nullptr;
}

Terrain::Terrain(const char * heightmap) : noise(*Texture::textures["noise"])
{
	m_heightmap = nullptr;

	loadHeightMap(heightmap);
}

Terrain::~Terrain()
{
	if (m_heightmap)
	delete[] m_heightmap;
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(2, m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

bool Terrain::loadHeightMap(const char * path)
{
	if (m_heightmap)
		delete[] m_heightmap;

	FILE * file = fopen(path, "rb");

	fread(&m_size, sizeof(int), 1, file);

	m_heightmap = new float[m_size*m_size * 4];
	fread(m_heightmap, sizeof(float), m_size*m_size * 4, file);

	fclose(file);

	float * grid = getGrid();
	unsigned int * indices = indexicate();

	glGenBuffers(2, m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, m_size * m_size * 2 * sizeof(float), grid, GL_STATIC_DRAW); // grid

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, m_size * m_size * 4 * sizeof(float), m_heightmap, GL_STATIC_DRAW); // heightmap

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (m_size - 1)*(m_size - 1) * 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	delete[] indices;
	delete[] grid;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(sizeof(float) * m_size * m_size));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	glBindVertexArray(0);

	return true;
}

unsigned int * Terrain::indexicate()
{
	unsigned int * indices = new unsigned int[(m_size - 1) * (m_size - 1) * 6];

	for (int i = 0; i < m_size - 1; i++)
		for (int j = 0; j < m_size - 1; j++)
		{
			int ind = (i * (m_size - 1) + j) * 6;

			indices[ind] = i * m_size + j;
			indices[ind + 1] = (i + 1) * m_size + j + 1;
			indices[ind + 2] = (i + 1) * m_size + j;
			indices[ind + 3] = i * m_size + j;
			indices[ind + 4] = i * m_size + j + 1;
			indices[ind + 5] = (i + 1) * m_size + j + 1;
		}
	return indices;
}

float * Terrain::getGrid()
{
	float * grid = new float[m_size *m_size * 2];

	for (int i = 0; i < m_size; i++)
		for (int j = 0; j < m_size; j++)
		{
			int ind = i * m_size + j;
			grid[ind * 2] = i;
			grid[ind * 2 + 1] = j;
		}
	return grid;
}

void Terrain::draw()
{
	noise.bind();
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, (m_size - 1)*(m_size - 1) * 6, GL_UNSIGNED_INT, 0);
}


float Terrain::findHeight(glm::vec2 position)
{
	float X = floor(position.x);
	float Y = floor(position.y);
	float x = position.x - X;
	float y = position.y - Y;

	int target = X*m_size + Y;
	float a = m_heightmap[target];
	float b = m_heightmap[target + 1];
	float c = m_heightmap[target + m_size];
	float d = m_heightmap[target + m_size + 1];


	float result = linearInterpolaton(
		linearInterpolaton(a, b, x), 
		linearInterpolaton(c, d, x), 
		y);

	return result;
}

glm::vec2 Terrain::findSlope(glm::vec2 position)
{
	float X = floor(position.x);
	float Y = floor(position.y);
	float x = position.x - X;
	float y = position.y - Y;


	int target = X*m_size + Y;

	if (x > y)
	{
		float a = m_heightmap[target];
		float c = m_heightmap[target + m_size];
		float d = m_heightmap[target + m_size + 1];

		return glm::vec2(c - a, d - c);
	}
	else
	{
		float a = m_heightmap[target];
		float b = m_heightmap[target + 1];
		float d = m_heightmap[target + m_size + 1];
		return glm::vec2(d - b, b - a);
	}
}
