#pragma once
#include "Scene.h"

class RenderEngine
{
	static GLuint shadowMapFBO;
	static GLuint shadowMap;

	static GLuint gFBO; 
	static GLuint gRBO;
	static GLuint gColor, gNormPos;
	
	static GLuint MatrixID;
	static GLuint ModelMatrixID;
	static GLuint ViewMatrixID;
	static GLuint gColorID;
	static GLuint gNormPosID;
	
	static GLuint LightDirectionID;
	
	static const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	static void renderSolids(Scene &scene, glm::mat4 &matrix);
	static void renderInstanced(Scene &scene, glm::mat4 &matrix);
	static void renderTerrain(Scene &scene, glm::mat4 &matrix);
	static void renderWater(Scene &scene, glm::mat4 &matrix);

	static void renderShadowMap(Scene & scene);
public:
	static void init();
	static void terminate();
	static void render(Scene & scene);
};

