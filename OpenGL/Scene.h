#pragma once
#include <vector>
#include "Node.h"
#include "NodeInstansed.h"
#include "Terrain.h"
#include "Camera.h"
#include "Water.h"
#include "Vegitation.h"

class Scene
{
public:

	Camera camera;
	Terrain terrain;
	Water * water;
	std::vector<Node*> solidObjects;
	std::vector<Node*> glowingObjects;

	//grassPatch* grasspatch;
	Vegitation * grass;

	//std::vector<NodeInstansed*> grass;
	std::vector<NodeInstansed*> solidObjectsInstanced;
	glm::mat4 lightMatrix;
	glm::vec3 SunDirection;


	void update();

	Scene();
	~Scene();
};

