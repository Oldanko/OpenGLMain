#pragma once
#include <vector>
#include "Node.h"
#include "NodeInstansed.h"
#include "Terrain.h"
#include "Camera.h"

class Scene
{
public:
	Camera camera;
	Terrain terrain;
	std::vector<Node*> solidObjects;
	std::vector<Node*> glowingObjects;
	std::vector<NodeInstansed*> grass;
	std::vector<NodeInstansed*> solidObjectsInstanced;
	glm::mat4 lightMatrix;
	glm::vec3 SunDirection;


	Scene();
	~Scene();
};

