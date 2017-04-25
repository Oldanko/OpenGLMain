#include "Scene.h"
#include "CameraFreeFloat.h"


void Scene::update()
{
	for (int i = 0; i < solidObjects.size(); i++)
		solidObjects[i]->calculateMatrices();
	for (int i = 0; i < glowingObjects.size(); i++)
		glowingObjects[i]->calculateMatrices();

	if (grass)
		grass->update(camera->position().x, camera->position().z);

	camera->update();

	if (water)
		water->calculateMatrices(camera->pov());
}

Scene::Scene()
{
	grass = nullptr;
	water = nullptr;

	camera = new CameraFreeFloat();
}


Scene::~Scene()
{
	if (camera)
		delete camera;
	for (int i = 0; i < solidObjects.size(); i++)
		delete solidObjects[i];
	for (int i = 0; i < solidObjectsInstanced.size(); i++)
		delete solidObjectsInstanced[i];
	for (int i = 0; i < glowingObjects.size(); i++)
		delete glowingObjects[i];
	if (grass)
		delete grass;
	if (water)
		delete water;
}
