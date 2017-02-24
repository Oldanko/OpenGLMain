#include "Scene.h"



void Scene::update()
{
	camera.update();
	if (water)
		water->calculateMatrices(camera);

	for (int i = 0; i < solidObjects.size(); i++)
		solidObjects[i]->calculateMatrices();
	for (int i = 0; i < glowingObjects.size(); i++)
		glowingObjects[i]->calculateMatrices();


}

Scene::Scene()
{

	water = nullptr;
}


Scene::~Scene()
{
	for (int i = 0; i < solidObjects.size(); i++)
		delete solidObjects[i];
	for (int i = 0; i < solidObjectsInstanced.size(); i++)
		delete solidObjectsInstanced[i];
	for (int i = 0; i < glowingObjects.size(); i++)
		delete glowingObjects[i];
	for (int i = 0; i < grass.size(); i++)
		delete grass[i];
	delete water;
}
