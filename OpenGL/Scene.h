#pragma once
#include <vector>
#include "Node.h"
#include "NodeInstansed.h"
#include "Terrain.h"
#include "Camera.h"
#include "Water.h"
#include "Vegitation.h"
#include "PointOfView.h"

class Scene
{
public:

	Camera camera;

	PointOfView pov;
	
	Terrain terrain;
	Water * water;
	std::vector<Node*> solidObjects;
	std::vector<Node*> glowingObjects;

	Vegitation * grass;
	std::vector<NodeInstansed*> solidObjectsInstanced;
	glm::mat4 lightMatrix;
	glm::vec3 SunDirection;


	void update();
	void powUpdate();

	Scene();
	~Scene();
};

