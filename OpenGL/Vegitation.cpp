#include "Vegitation.h"

#include <iostream>

#include <thread>
#include <mutex>
#include <glm\gtx\transform.hpp>

using namespace std;


ubyte elem[3][6] = 
{ 
	{ 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 1, 1, 0, 0 },
	{ 1, 2, 2, 2, 1, 0 } 
};

ubyte mask[6] = { 1, 2, 2, 2, 1, 0 };

const int grassPatch::q = 1024;
const int grassPatch::size = 32;
glm::vec2 grassPatch::coords[q];

void grassPatch::init()
{
	for (int i = 0; i < q; i++)
	{
		coords[i].x = (float)size * rand() / RAND_MAX;
		coords[i].y = (float)size * rand() / RAND_MAX;
	}
}

void grassPatch::terminate()
{
	//glDeleteBuffers(1, &m_matrixBuffer);
}

grassPatch::grassPatch(int _x, int _y, Mesh & mesh, Texture & texture, Terrain & terrain) : m_mesh(mesh), m_texture(texture), m_terrain(terrain)
{
	x = INT_MIN; y = INT_MIN;

	glGenBuffers(1, &m_matrixBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_matrixBuffer);
	glBufferData(GL_ARRAY_BUFFER, q * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

	x = _x;
	y = _y;

	update_needed = false;
	updated = true;

	matrices = new glm::mat4[q];
	for (int i = 0; i < q; i++)
	{
		matrices[i][3][0] = coords[i].x + x*size;
		matrices[i][3][2] = coords[i].y + y*size;
		matrices[i][3][1] = m_terrain.findHeight(coords[i] + glm::vec2(x*size, y*size));

		auto slope = m_terrain.findSlope(coords[i] + glm::vec2(x*size, y*size));

		matrices[i] *= glm::rotate((float)atan(slope.x), glm::vec3(0, 0, 1))*
			glm::rotate((float)atan(-slope.y), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(2, 1.5, 2));
	}

	glGenBuffers(1, &m_matrixBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_matrixBuffer);
	glBufferData(GL_ARRAY_BUFFER, q * sizeof(glm::mat4), &matrices[0], GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	m_mesh.bindVBO();
	m_mesh.bindEBO();

	GLsizei vec4Size = sizeof(glm::vec4);
	glBindBuffer(GL_ARRAY_BUFFER, m_matrixBuffer);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}

grassPatch::~grassPatch()
{

	delete[] matrices;
	glDeleteBuffers(1, &m_matrixBuffer);
	glDeleteVertexArrays(1, &m_vao);
}

void grassPatch::init(int _x, int _y)
{
	if (x == _x && y == _y)
		return;

	x = _x;
	y = _y;

	update_mtx.lock();
	update_needed = true;
	loaded = false;
	update_mtx.unlock();
	updated = false;
}

void grassPatch::draw()
{
	update_mtx.lock();
	if (!updated)
	{
		update_mtx.unlock();
		return;
	}
	update_mtx.unlock();

	m_texture.bind();
	glBindVertexArray(m_vao);
	glDrawElementsInstanced(GL_TRIANGLES, m_mesh.indicesNum(), GL_UNSIGNED_INT, 0, q);
}

void grassPatch::updateAsynch()
{
	update_mtx.lock();
	if (!update_needed)
	{
		update_mtx.unlock();
		return;
	}

	update_needed = false;
	update_mtx.unlock();

	for (int i = 0; i < q; i++)
	{
		if (i % 32 == 0)
		{
			update_mtx.lock();
			if (update_needed)
			{
				update_mtx.unlock();
				return;
			}
			update_mtx.unlock();
		}

		matrices[i] = glm::mat4();

		matrices[i][3][0] = coords[i].x + x*size;
		matrices[i][3][2] = coords[i].y + y*size;
		matrices[i][3][1] = m_terrain.findHeight(coords[i] + glm::vec2(x*size, y*size));

		auto slope = m_terrain.findSlope(coords[i] + glm::vec2(x*size, y*size));

		matrices[i] *= glm::rotate((float)atan(slope.x), glm::vec3(0, 0, 1))*
			glm::rotate((float)atan(-slope.y), glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(2, 1.5, 2));
	}


	update_mtx.lock();
	if (update_needed)
	{
		update_mtx.unlock();
		return;
	}
	loaded = true;
	update_mtx.unlock();

	//cout << "Done\n";
}


bool grassPatch::update()
{
	update_mtx.lock();
	if (loaded && !updated)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_matrixBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, q * sizeof(glm::mat4), &matrices[0]);
		//cout << glGetError() << "\n";
		update_mtx.unlock();
		updated = true;
		return true;
	}
	update_mtx.unlock();
	return false;
}



Vegitation::Vegitation(Mesh & mesh, Texture & texture, Terrain & terrain)
{
	x = 0;
	y = 0;

	int _x = x - tilesNum / 2 + 1;
	int _y = y - tilesNum / 2 + 1;

	for (int i = 0; i < tilesNum; i++)
		for (int j = 0; j < tilesNum; j++)
			tiles[i][j] = new grassPatch(_x + i, _y + j, mesh, texture, terrain);
	updates = true;
	th = thread(&Vegitation::updateAsync, this);
}

Vegitation::~Vegitation()
{
	
	updates_mtx.lock();
	updates = false;
	updates_mtx.unlock();
	th.join();
	for (int i = 0; i < tilesNum; i++)
		for (int j = 0; j < tilesNum; j++)
		{
			if (tiles[i][j])
				delete tiles[i][j];
		}
}

void Vegitation::North()
{
	y++;

	int _x = x - tilesNum / 2 + 1;
	int _y = y - tilesNum / 2 + 1;

	grassPatch * buff[tilesNum];

	for (int i = 0; i < tilesNum; i++)
		buff[i] = tiles[i][0];

	for (int i = 0; i < tilesNum; i++)
		for (int j = 0; j < tilesNum - 1; j++)
		{
			tiles[i][j] = tiles[i][j + 1];
		}

	for (int i = 0; i < tilesNum; i++)
		tiles[i][tilesNum - 1] = buff[i];


	for (int i = 0; i < tilesNum; i++)
		for (int j = 0; j < tilesNum; j++)
			if (elem[mask[i]][j])
				tiles[i][j]->init(_x + i, _y + j);
}

void Vegitation::South()
{
	y--;

	int _x = x - tilesNum / 2 + 1;
	int _y = y - tilesNum / 2 + 1;

	grassPatch * buff[tilesNum];

	for (int i = 0; i < tilesNum; i++)
		buff[i] = tiles[i][tilesNum - 1];

	for (int i = 0; i < tilesNum; i++)
		for (int j = tilesNum - 1; j > 0; j--)
		{
			tiles[i][j] = tiles[i][j - 1];
		}

	for (int i = 0; i < tilesNum; i++)
		tiles[i][0] = buff[i];


	for (int i = 0; i < tilesNum; i++)
		for (int j = 0; j < tilesNum; j++)
			if (elem[mask[i]][j])
				tiles[i][j]->init(_x + i, _y + j);
}

void Vegitation::East()
{
	x++;

	int _x = x - tilesNum / 2 + 1;
	int _y = y - tilesNum / 2 + 1;

	grassPatch * buff[tilesNum];

	for (int i = 0; i < tilesNum; i++)
		buff[i] = tiles[0][i];

	for (int i = 0; i < tilesNum - 1; i++)
		for (int j = 0; j < tilesNum; j++)
		{
			tiles[i][j] = tiles[i + 1][j];
		}

	for (int i = 0; i < tilesNum; i++)
		tiles[tilesNum - 1][i] = buff[i];


	for (int i = 0; i < tilesNum; i++)
		for (int j = 0; j < tilesNum; j++)
			if (elem[mask[i]][j])
				tiles[i][j]->init(_x + i, _y + j);
}

void Vegitation::West()
{
	x--;

	int _x = x - tilesNum / 2 + 1;
	int _y = y - tilesNum / 2 + 1;

	grassPatch * buff[tilesNum];

	for (int i = 0; i < tilesNum; i++)
		buff[i] = tiles[tilesNum - 1][i];

	for (int i = tilesNum - 1; i > 0; i--)
		for (int j = 0; j < tilesNum; j++)
		{
			tiles[i][j] = tiles[i - 1][j];
		}

	for (int i = 0; i < tilesNum; i++)
		tiles[0][i] = buff[i];


	for (int i = 0; i < tilesNum; i++)
		for (int j = 0; j < tilesNum; j++)
			if (elem[mask[i]][j])
				tiles[i][j]->init(_x + i, _y + j);
}

void Vegitation::update(float _x, float _y)
{
	int X = _x / 32;
	int Y = _y / 32;

	if (y < Y)
	{
		North();
		return;
	}
	if (y > Y)
	{
		South();
		return;
	}
	if (x < X)
	{
		East();
		return;
	}
	if (x > X)
	{
		West();
		return;
	}
	
	for (int i = 0; i < tilesNum; i++)
	{
		for (auto tile : tiles[i])
		{
			if (tile->update())
				return;
		}
	}

}

void Vegitation::updateAsync()
{

	updates_mtx.lock();
	while (updates)
	{
		updates_mtx.unlock();
		for (int i = 0; i < tilesNum; i++)
		{
			for (auto tile : tiles[i])
			{
				tile->updateAsynch();
			}
		}
		updates_mtx.lock();
	}
	updates_mtx.unlock();
}

void Vegitation::draw()
{
	for(int i = 0; i < tilesNum; i++)
		for (int j = 0; j < tilesNum; j++)
		{
			if (elem[mask[i]][j])
				tiles[i][j]->draw();
		}
}
