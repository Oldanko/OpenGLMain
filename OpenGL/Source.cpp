#include <vector>
#include <fstream>
#include <string>
#include <SOIL\SOIL.h>
#include <time.h>

#include "WindowManager.h"
#include "Camera.h"
#include "Controls.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shapes.h"
#include "Node.h"
#include "NodeInstansed.h"
#include "ShaderManager.h"
#include "Scene.h"
#include "RenderEngine.h"

#include "GUI.h"

#include <glm\gtx\vector_angle.hpp>


#include <chrono>

using namespace std::chrono;

#define NOW_MS duration_cast<microseconds>(system_clock::now().time_since_epoch())


int main()
{
	WindowManager::init();
	Controls::init();
	ShaderManager::init();
	RenderEngine::init();

	srand(time(0));

	//Resources Managing
	std::vector<float> vertices;
	std::vector<unsigned int> elements;

	loadGrassStar(vertices, elements);
	Mesh grass(vertices, elements);

	loadPlane(vertices, elements);
	Mesh plain(vertices, elements);

	loadBox(vertices, elements);
	Mesh box(vertices, elements);


	Texture::textures["box"] = new Texture("resources/box.png",
		GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	Texture::textures["grass_a"] = new Texture("resources/billboardgrass.png",
		GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	Texture::textures["noise"] = new Texture("resources/noiseTexture.png",
		GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	
	std::vector<glm::mat4> matrices;

	for (int i = 0; i < 100000; i++)
	{
		int r = rand();
		int r2 = rand();
		glm::mat4 matrix =

			glm::translate(glm::vec3(sin(r) * 200, 0, cos(r) * 200) + glm::vec3(sin(r2) * 50, sin(rand()) * 2, cos(r2) * 50)) *
			glm::rotate(float(rand()), glm::vec3(rand(), rand(), rand())) ;
		matrices.push_back(matrix);
	}

	Scene scene;
	scene.solidObjects.push_back(new Node(box, *Texture::textures["box"],
		glm::vec3(0), glm::vec3(rand(), rand(), rand()), glm::vec3(150)));
	
	scene.solidObjectsInstanced.push_back(new NodeInstansed(box, *Texture::textures["box"], matrices));

	scene.glowingObjects.push_back(new Node(box, *Texture::textures["box"],
		glm::vec3(50000, 2500, 50000), glm::vec3(0.0), glm::vec3(2000)));

	scene.SunDirection = glm::vec3(1.0, 1.0, 1.0);

	scene.lightMatrix =
		glm::ortho(-300.0f, 300.0f, -300.0f, 300.0f, 10.0f, 500.0f) *
		glm::lookAt(glm::vec3(250.0, 12.5, 250.0),
			glm::vec3(0.0, 0.0, 0.0),
			glm::vec3(0.0, 1.0, 0.0));

	Scene scene2;

	scene2.terrain.loadHeightMap("resources/heightmap.hm");
	scene2.SunDirection = glm::vec3(1, 0.5, 1);

	scene2.solidObjects.push_back(new Node(box, *Texture::textures["box"],
		glm::vec3(256, 10, 256), glm::vec3(0.0, glm::degrees(90.0), 0.0), glm::vec3(15)));



	scene2.lightMatrix =
		glm::ortho(-350.0f, 350.0f, -350.0f, 350.0f, 10.0f, 1000.0f) *
		glm::lookAt(glm::vec3(600.0, 600.0, 600.0),
			glm::vec3(250.0, 0.0, 250.0),
			glm::vec3(0.0, 1.0, 0.0));

	//Plant Grass
	matrices = std::vector<glm::mat4>(0);
	std::vector<glm::mat4> matrices2;
	for (int i = 0; i < 10000; i++)
	{
		float x = (float)(scene2.terrain.size() - 1) * rand() / RAND_MAX / 2;
		float y = (float)(scene2.terrain.size() - 1) * rand() / RAND_MAX / 2;
		float h = scene2.terrain.findHeight(glm::vec2(x, y));
		
		glm::vec2 slope = scene2.terrain.findSlope(glm::vec2(x, y));

		if (glm::length(slope) > 0.3)
		{
			i--;
			continue;
		}

		if (h < 2.0f || (h < 2.5 && rand() % 2 == 0))
		{
			if (h > 1.0 || (h > 0.8 && rand() % 2 == 0))
			{
				if (rand() % 3 == 0)
				{
					float r = float(rand()) * 0.1 / INT_MAX;

					glm::mat4 matrix =
						glm::translate(glm::vec3(x, h, y)) *
						glm::rotate((float)atan(slope.x), glm::vec3(0, 0, 1))*
						glm::rotate((float)atan(-slope.y), glm::vec3(1, 0, 0))*
						glm::rotate(float(rand()), glm::vec3(0, 1, 0))*
						glm::scale(glm::vec3(0.2 + r, 0.2 + r, 0.2 + r));
					matrices2.push_back(matrix);
				}
				continue;
			}
			i--;
			continue;
		}

		glm::mat4 matrix =
			glm::translate(glm::vec3(x, h, y)) *
			glm::rotate((float)atan(slope.x), glm::vec3(0, 0, 1))*
			glm::rotate((float)atan(-slope.y), glm::vec3(1, 0, 0))*
			glm::rotate(float(rand()), glm::vec3(0, 1, 0))*
			glm::scale(glm::vec3(5.0, 2.0, 5.0));
		matrices.push_back(matrix);
	}

	scene2.grass.push_back(new NodeInstansed(grass, *Texture::textures["grass_a"], matrices));
	//scene2.grass.push_back(new NodeInstansed(grass, grass1_texture, matrices2));

	//Add magic boxes of levitation
	matrices = std::vector<glm::mat4>();
	for (int i = 0; i < 200; i++)
	{
		float x = (float)(scene2.terrain.size() - 1) * rand() / RAND_MAX;
		float y = (float)(scene2.terrain.size() - 1) * rand() / RAND_MAX;

		glm::mat4 matrix =
			glm::translate(glm::vec3(x, 20, y));
		matrices.push_back(matrix);
	}

	scene2.solidObjectsInstanced.push_back(new NodeInstansed(box, *Texture::textures["box"], matrices));

	scene2.water = new Water(glm::vec2(0), glm::vec2(512), 1.4f);

	Scene scene3;

	scene3.terrain.loadHeightMap("resources/heightmap.hm");
	scene3.grass.push_back(new NodeInstansed(grass, *Texture::textures["grass_a"], matrices));
	scene3.SunDirection = glm::vec3(1, 1, 1);
	scene3.lightMatrix =
		glm::ortho(-350.0f, 350.0f, -350.0f, 350.0f, 10.0f, 1000.0f) *
		glm::lookAt(glm::vec3(600.0, 600.0, 600.0),
			glm::vec3(250.0, 0.0, 250.0),
			glm::vec3(0.0, 1.0, 0.0));


	Scene * theScene = &scene2;

	slider sld(glm::vec2(0.45, 0.8), 0.5);
	sld.setValue(0.15);

	do
	{

		RenderEngine::render(*theScene);
		theScene->update();

		sld.update();
		if (theScene->water)
			theScene->water->setHeight(sld.value() * 10);

		glDisable(GL_CULL_FACE);
		glUseProgram(ShaderManager::program2D);
		sld.draw();
		glEnable(GL_CULL_FACE);

		Controls::update();

	} while (WindowManager::update());

	ShaderManager::terminate();
	RenderEngine::terminate();
	Texture::terminate();



	return 0;
}