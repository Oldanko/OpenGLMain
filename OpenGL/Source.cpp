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
	Texture::init();
	grassPatch::init();


	srand(time(0));

	//Loading meshes
	std::vector<float> vertices;
	std::vector<unsigned int> elements;

	loadGrassStar(vertices, elements);
	Mesh grass(vertices, elements);

	loadBox(vertices, elements);
	Mesh box(vertices, elements);

	loadPlain(vertices, elements);
	Mesh plain(vertices, elements);


	Scene scene;

	scene.terrain.loadHeightMap("resources/heightmap.hm");
	//scene.character.terrain = &scene.terrain;

	//Setting Sun configurations. 
	scene.SunDirection = glm::vec3(1, 0.5, 1);

	scene.lightMatrix = //TODO: should be updated each frame depending on a camera position
		glm::ortho(-350.0f, 350.0f, -350.0f, 350.0f, 10.0f, 1000.0f) *
		glm::lookAt(glm::vec3(600.0, 600.0, 600.0),
			glm::vec3(250.0, 0.0, 250.0),
			glm::vec3(0.0, 1.0, 0.0));


	scene.solidObjects.push_back(new Node(box, *Texture::textures["box"],
		glm::vec3(256, 10, 256), glm::vec3(0.0, glm::degrees(90.0), 0.0), glm::vec3(15)));

	//Adding Grass
	scene.grass = new Vegitation(grass, *Texture::textures["grass"], scene.terrain);

	//Adding magic boxes of levitation
	std::vector<glm::mat4> matrices = std::vector<glm::mat4>();
	for (int i = 0; i < 200; i++)
	{
		float x = (float)(scene.terrain.size() - 1) * rand() / RAND_MAX;
		float y = (float)(scene.terrain.size() - 1) * rand() / RAND_MAX;

		glm::mat4 matrix =
			glm::translate(glm::vec3(x, 20, y));
		matrices.push_back(matrix);
	}
	scene.solidObjectsInstanced.push_back(new NodeInstansed(box, *Texture::textures["box"], matrices));

	//Adding water
	scene.water = new Water(glm::vec2(0), glm::vec2(512), 1.4f);



	billboard bboard[2];
	bboard[0].m_position = glm::vec2(-0.95f, -0.95f);
	bboard[0].m_size = glm::vec2(0.4, 0.4);

	bboard[1].m_position = glm::vec2(-0.45f, -0.95f);
	bboard[1].m_size = glm::vec2(0.4, 0.4);

	unsigned int frame = 0;

	glfwSetCursorPos(WindowManager::window(), WindowManager::width() / 2, WindowManager::height() / 2);
	do
	{
		frame++;
		RenderEngine::render(scene);

		glUseProgram(ShaderManager::program2D);
		glActiveTexture(GL_TEXTURE0);

		glDisable(GL_DEPTH_TEST);
		//Texture::textures["water_normal_map"]->bind();
		//bboard[0].draw();
		//Texture::textures["gColorRefl"]->bind();
		//bboard[1].draw();
		glEnable(GL_DEPTH_TEST);

		Controls::update();
		scene.update();

	} while (WindowManager::update());

	grassPatch::terminate();
	ShaderManager::terminate();
	RenderEngine::terminate();
	Texture::terminate();

	return 0;
}