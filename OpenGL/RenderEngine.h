#pragma once
#include "Scene.h"

class RenderEngine
{
	static GLuint depthMapFBO;
	static GLuint depthMap;

	static GLuint gFBO; 
	static GLuint gRBO;
	static GLuint gColor, gNormal, gPosition, gMaterial;
	
	static GLuint MatrixID;
	static GLuint ModelMatrixID;
	static GLuint ViewMatrixID;
	static GLuint gColorID;
	static GLuint gNormalID;
	static GLuint gPositionID;
	
	static GLuint LightDirectionID;
	
	static const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	static void renderSolids(Scene &scene, glm::mat4 &matrix);
	static void renderInstanced(Scene &scene, glm::mat4 &matrix);
	static void renderTerrain(Scene &scene, glm::mat4 &matrix);
	static void renderWater(Scene &scene, glm::mat4 &matrix);

	static void renderSolidsReflection(Scene &scene, glm::mat4 &matrix, bool reflection);
	static void renderInstancedReflection(Scene &scene, glm::mat4 &matrix, bool reflection);
	static void renderTerrainReflection(Scene &scene, glm::mat4 &matrix, bool reflection);

	static void renderShadowMap(Scene & scene);
public:
	static void init();
	static void terminate();
	/*static void draw(Scene & scene);
	static void drawShaded(Scene & scene);*/
	static void render(Scene & scene);
};

