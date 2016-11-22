#pragma once
#include "Scene.h"

class RenderEngine
{
	static GLuint depthMapFBO;
	static GLuint depthMap;
	static const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	static void renderTerrain(Scene &scene, glm::mat4 &matrix, bool shadow);
	static void renderInstanced(Scene &scene, glm::mat4 &matrix, bool shadow);
	static void renderSolids(Scene &scene, glm::mat4 &matrix, bool shadow);
	static void renderGlow(Scene &scene, glm::mat4 matrix);
	static void renderShadowMap(Scene & scene);
	static void renderWater(Scene &scene, glm::mat4 &matrix, bool shadow);
public:
	static void init();
	static void terminate();
	static void draw(Scene & scene);
	static void drawShaded(Scene & scene);
};

