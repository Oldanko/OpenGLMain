#pragma once
#include <GL\glew.h>

class ShaderManager
{
	static GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
public:

	static GLuint program;
	static GLuint programInstanced;
	static GLuint programTerrain;
	static GLuint programWater;

	static GLuint programReflection;
	static GLuint programInstancedReflection;
	static GLuint programTerrainReflection;

	static GLuint programDepth;
	static GLuint programInstancedDepth;
	static GLuint programTerrainDepth;

	static GLuint program2D;
	
	static GLuint programLighting;


	static void init();
	static void terminate();
	
};

