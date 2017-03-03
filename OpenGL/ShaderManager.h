#pragma once
#include <GL\glew.h>

class ShaderManager
{
	static GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
public:

	static GLuint program;
	static GLuint programInstanced;
	static GLuint programTerrain;

	static GLuint programDepth;
	static GLuint programDepthInstanced;
	static GLuint programTerrainDepth;

	static GLuint program2D;
	
	static GLuint programLighting;

	//static GLuint programWater;
	/*static GLuint programGlow;


	static GLuint program2D;

	static GLuint programShadow;
	//static GLuint programShadowInstanced;

	static GLuint programTerrain;
	//static GLuint programTerrainShadow;

	static GLuint programWater;
	//static GLuint programWaterShadow;*/

	static void init();
	static void terminate();
	
};

