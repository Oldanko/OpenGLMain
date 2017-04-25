#pragma once

#include <iostream>

#include <thread>
#include <mutex>
#include <vector>

#include <chrono>

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "Mesh.h"
#include "Texture.h"
#include "Terrain.h"

using namespace std;

typedef unsigned char ubyte;
typedef unsigned short ushort;
typedef unsigned int uint;

class grassPatch
{
	static glm::vec2 coords[];

	GLuint m_matrixBuffer;
	GLuint m_vao;
	GLuint m_heightBuffer;
	Mesh &m_mesh;
	Texture &m_texture;
	Terrain &m_terrain;
	glm::mat4 *matrices;
	int x;
	int y;
	bool update_needed;
	bool loaded;
	bool updated;

	mutex update_mtx;

public:
	static const int q;
	static const int size;

	static void init();
	static void terminate();
	
	grassPatch(int _x, int _y, Mesh & mesh, Texture & texture, Terrain & terrain);
	~grassPatch();
	void init(int _x, int _y);
	void draw();
	bool update();
	void updateAsynch();
};

const unsigned int tilesNum = 6;

class Vegitation
{
	int x;
	int y;
	thread th;
public:
	bool updates;
	mutex updates_mtx;

	grassPatch * tiles[tilesNum][tilesNum];
	Vegitation(Mesh & mesh, Texture & texture, Terrain & terrain);
	~Vegitation();

	void North();
	void South();
	void East();
	void West();
	void update(float x, float y);
	void updateAsync();
	void draw();
};