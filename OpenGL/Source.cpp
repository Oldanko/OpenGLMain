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

	//Resources Managing
	std::vector<float> vertices;
	std::vector<unsigned int> elements;


	loadGrassStar(vertices, elements);
	Mesh grass(vertices, elements);
	
	loadBox(vertices, elements);
	Mesh box(vertices, elements);

	loadPlain(vertices, elements);
	Mesh plain(vertices, elements);
	
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
	
	/*scene2.solidObjects.push_back(new Node(plain, *Texture::textures["wall_red"],
		glm::vec3(70, 0, 70), glm::vec3(0, 0, 0), glm::vec3(32))); 
	scene2.solidObjects.push_back(new Node(plain, *Texture::textures["wall_red"],
			glm::vec3(102, 0, 70), glm::vec3(0, 3.14, 0), glm::vec3(32)));
	
	scene2.solidObjects.push_back(new Node(plain, *Texture::textures["wall_blue"],
		glm::vec3(70, 0, 102), glm::vec3(0, 1.57, 0), glm::vec3(32)));
	scene2.solidObjects.push_back(new Node(plain, *Texture::textures["wall_blue"],
		glm::vec3(70, 0, 70), glm::vec3(0, -1.57, 0), glm::vec3(32)));

	scene2.solidObjects.push_back(new Node(plain, *Texture::textures["wall_yellow"],
		glm::vec3(70+32, -10, 70), glm::vec3(0, -1.57, 0), glm::vec3(32)));
	scene2.solidObjects.push_back(new Node(plain, *Texture::textures["wall_yellow"],
		glm::vec3(102, -10, 102), glm::vec3(0, 1.57, 0), glm::vec3(32)));

	scene2.solidObjects.push_back(new Node(plain, *Texture::textures["wall_white"],
		glm::vec3(70, 0, 102), glm::vec3(0, 0, 0), glm::vec3(32)));
	scene2.solidObjects.push_back(new Node(plain, *Texture::textures["wall_white"],
		glm::vec3(102, 0, 102), glm::vec3(0, 3.14, 0), glm::vec3(32)));*/

	scene2.lightMatrix =
		glm::ortho(-350.0f, 350.0f, -350.0f, 350.0f, 10.0f, 1000.0f) *
		glm::lookAt(glm::vec3(600.0, 600.0, 600.0),
			glm::vec3(250.0, 0.0, 250.0),
			glm::vec3(0.0, 1.0, 0.0));

	//Add Grass
	scene2.grass = new Vegitation(grass, *Texture::textures["grass"], scene2.terrain);

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


	Scene * theScene = &scene2;

	billboard bboard[4];
	bboard[0].m_position = glm::vec2(-0.95f, -0.95f);
	bboard[0].m_size = glm::vec2(0.4, 0.4);
	
	bboard[1].m_position = glm::vec2(-0.45f, -0.95f);
	bboard[1].m_size = glm::vec2(0.4, 0.4);

	bboard[2].m_position = glm::vec2(0.05f, -0.95f);
	bboard[2].m_size = glm::vec2(0.4, 0.4);

	bboard[3].m_position = glm::vec2(0.5f, -0.95f);
	bboard[3].m_size = glm::vec2(0.4, 0.4);

	uint frame = 0;

	do
	{
		RenderEngine::render(*theScene);
		theScene->update();
		
		frame++;

		glDisable(GL_DEPTH_TEST);
		glUseProgram(ShaderManager::program2D);
		glActiveTexture(GL_TEXTURE0);

		//Texture::textures["water_normal_map"]->bind();
		//bboard[0].draw();
		//Texture::textures["gColorRefl"]->bind();
		//bboard[1].draw();
		//Texture::textures[""]->bind();
		//bboard[2].draw();
		//Texture::textures["Refr"]->bind();
		//bboard[3].draw();

		glEnable(GL_DEPTH_TEST);

		Controls::update();

	} while (WindowManager::update());

	grassPatch::terminate();
	ShaderManager::terminate();
	RenderEngine::terminate();
	Texture::terminate();
	
	return 0;
}