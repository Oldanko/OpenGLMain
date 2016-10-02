#include "Scene.h"



Scene::Scene()
{
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
}
