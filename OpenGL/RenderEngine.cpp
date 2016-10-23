#include "RenderEngine.h"
#include "ShaderManager.h"


GLuint RenderEngine::depthMapFBO;
GLuint RenderEngine::depthMap;

void RenderEngine::renderTerrain(Scene &scene, glm::mat4 &matrix, bool shadow)
{
	GLuint *program;
	
	shadow ?
		//prepare shadow
		program = &ShaderManager::programTerrainShadow
		:
		//prepare no shadow
		program = &ShaderManager::programTerrain;

	glUseProgram(*program);

	if (shadow)
		glUniformMatrix4fv(glGetUniformLocation(*program, "lightMatrix"), 1, GL_FALSE, &scene.lightMatrix[0][0]);

	scene.water ?
		glUniform2f(glGetUniformLocation(*program, "heightTest"), 1, scene.water->height())
		:
		glUniform2f(glGetUniformLocation(*program, "heightTest"), 0, 0);

	glUniform3f(glGetUniformLocation(*program, "lightDirection"), scene.SunDirection.x, scene.SunDirection.y, scene.SunDirection.z);
	glUniformMatrix4fv(glGetUniformLocation(*program, "MVP"), 1, GL_FALSE, &matrix[0][0]);
	scene.terrain.draw();
}

void RenderEngine::renderInstanced(Scene & scene, glm::mat4 & matrix, bool shadow)
{
	GLuint *program;

	shadow ?
		program = &ShaderManager::programShadowInstanced
		:
		program = &ShaderManager::programInstanced;


	glUseProgram(*program);
	glUniform3f(glGetUniformLocation(*program, "lightDirection"), scene.SunDirection.x, scene.SunDirection.y, scene.SunDirection.z);

	glUniform1i(glGetUniformLocation(*program, "shadowMap"), 0);
	glUniform1i(glGetUniformLocation(*program, "diffuseMap"), 1);

	
	if (shadow)
	{
		glUniformMatrix4fv(glGetUniformLocation(*program, "lightMatrix"), 1, GL_FALSE, &(scene.lightMatrix)[0][0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glActiveTexture(GL_TEXTURE1);
	}

	glUniformMatrix4fv(glGetUniformLocation(*program, "VP"), 1, GL_FALSE, &matrix[0][0]);
	for (int i = 0; i < scene.solidObjectsInstanced.size(); i++)
	{
		scene.solidObjectsInstanced[i]->draw();
	}

	glDisable(GL_CULL_FACE);
	for (int i = 0; i < scene.grass.size(); i++)
		scene.grass[i]->draw();
	glEnable(GL_CULL_FACE);
}

void RenderEngine::renderShadowMap(Scene & scene)
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_FRONT);

	glUseProgram(ShaderManager::programDepth);

	for (int i = 0; i < scene.solidObjects.size(); i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programDepth, "MVP"), 1, GL_FALSE, &(scene.lightMatrix*scene.solidObjects[i]->getModelMatrix())[0][0]);
		scene.solidObjects[i]->draw();
	}

	glUseProgram(ShaderManager::programDepthInstanced);
	glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programDepthInstanced, "VP"), 1, GL_FALSE, &scene.lightMatrix[0][0]);

	for (int i = 0; i < scene.solidObjectsInstanced.size(); i++)
	{
		scene.solidObjectsInstanced[i]->draw();
	}
	glUseProgram(ShaderManager::programTerrainDepth);
	glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programTerrainDepth, "VP"), 1, GL_FALSE, &scene.lightMatrix[0][0]);
	scene.terrain.draw();
		
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCullFace(GL_BACK);
}

void RenderEngine::init()
{
	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(ShaderManager::programTerrainShadow);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	

}

void RenderEngine::terminate()
{
	glDeleteFramebuffers(1, &depthMapFBO);
	glDeleteTextures(1, &depthMap);
}

void RenderEngine::draw(Scene & scene)
{
	// Water FBO test
	glViewport(0, 0, 1280, 720);

	glm::mat4 MVP = WindowManager::projectionMatrix() * scene.camera.ViewMatrix();

	scene.water->bindFrameBuffer();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(11 / 255.0, 176 / 255.0, 226 / 255.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(ShaderManager::programTerrain);

	glUniform3f(glGetUniformLocation(ShaderManager::programTerrain, "lightDirection"), scene.SunDirection.x, scene.SunDirection.y, scene.SunDirection.z);
	glUniform2f(glGetUniformLocation(ShaderManager::programTerrain, "heightTest"), 0, scene.water->height()); // change 0 to 1 to render above water
	glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programTerrain, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	

	scene.terrain.draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//End Water FBO test

	glViewport(0, 0, WindowManager::width(), WindowManager::height());

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(11/ 255.0, 176/ 255.0, 226 / 255.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glm::mat4 VP = WindowManager::projectionMatrix() * scene.camera.ViewMatrix();

	glUseProgram(ShaderManager::program);
	glUniform3f(glGetUniformLocation(ShaderManager::program, "lightDirection"), scene.SunDirection.x, scene.SunDirection.y, scene.SunDirection.z);
	for (int i = 0; i < scene.solidObjects.size(); i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(ShaderManager::program, "MVP"), 1, GL_FALSE, &(VP*scene.solidObjects[i]->getModelMatrix())[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(ShaderManager::program, "modelMatrix"), 1, GL_FALSE, &scene.solidObjects[i]->getModelMatrix()[0][0]);
		scene.solidObjects[i]->draw();
	}

	renderInstanced(scene, VP, false);

	glUseProgram(ShaderManager::programGlow);
	glUniform3f(glGetUniformLocation(ShaderManager::programGlow, "lightColor"), 3.0, 3.0, 3.0);
	for (int i = 0; i < scene.glowingObjects.size(); i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programGlow, "MVP"), 1, GL_FALSE, &(VP* scene.glowingObjects[i]->getModelMatrix())[0][0]);
		scene.glowingObjects[i]->draw();
	}

	renderTerrain(scene, VP, false);

	glUseProgram(ShaderManager::programWater);
	glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programWater, "VP"), 1, GL_FALSE, &VP[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programWater, "V"), 1, GL_FALSE, &scene.camera.ViewMatrix()[0][0]);
	scene.water->bindReflectionTexture();
	scene.water->draw();
}

void RenderEngine::drawShaded(Scene & scene)
{
	renderShadowMap(scene);

	glm::mat4 MVP = WindowManager::projectionMatrix() * scene.water->m_reflectionMatrix;
	//glm::mat4 MVP = scene.lightMatrix;

	if (scene.water)
	{
		// Water FBO test

		glViewport(0, 0, 1280, 720);


		glBindFramebuffer(GL_FRAMEBUFFER, scene.water->m_fbo);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glClearColor(11 / 255.0, 176 / 255.0, 226 / 255.0, 1.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(ShaderManager::programTerrain);

		glUniform3f(glGetUniformLocation(ShaderManager::programTerrain, "lightDirection"), scene.SunDirection.x, scene.SunDirection.y, scene.SunDirection.z);
		glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programTerrain, "MVP"), 1, GL_FALSE, &MVP[0][0]);
		glUniform1f(glGetUniformLocation(ShaderManager::programTerrain, "heightOffset"), -scene.water->height());
		glUniform2f(glGetUniformLocation(ShaderManager::programTerrain, "heightTest"), 1, scene.water->height());
		scene.terrain.draw();

		glUseProgram(ShaderManager::programShadowInstanced);
		glUniform1f(glGetUniformLocation(ShaderManager::programShadowInstanced, "heightOffset"), -scene.water->height());
		renderInstanced(scene, MVP, true);

		glUseProgram(ShaderManager::programShadow);
		glUniform3f(glGetUniformLocation(ShaderManager::programShadow, "lightDirection"), scene.SunDirection.x, scene.SunDirection.y, scene.SunDirection.z);

		glUniform1i(glGetUniformLocation(ShaderManager::programShadow, "shadowMap"), 0);
		glUniform1i(glGetUniformLocation(ShaderManager::programShadow, "diffuseMap"), 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		glActiveTexture(GL_TEXTURE1);

		glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programShadow, "lightMatrix"), 1, GL_FALSE, &(scene.lightMatrix)[0][0]);

		for (int i = 0; i < scene.solidObjects.size(); i++)
		{
			glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programShadow, "MVP"), 1, GL_FALSE, &(MVP*scene.solidObjects[i]->getModelMatrix())[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programShadow, "modelMatrix"), 1, GL_FALSE, &scene.solidObjects[i]->getModelMatrix()[0][0]);
			//box_node.draw();
			scene.solidObjects[i]->draw();
		}

		//End Water FBO test
	}

	glViewport(0, 0, WindowManager::width(), WindowManager::height());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(11 / 255.0, 176 / 255.0, 226 / 255.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	MVP = WindowManager::projectionMatrix() * scene.camera.ViewMatrix();
	//MVP = WindowManager::projectionMatrix() * scene.water->m_reflectionMatrix;
	//glm::mat4 MVP = scene.lightMatrix;



	// = = = = = = DRAW = = = = = =
	glUseProgram(ShaderManager::programShadow);
	glUniform3f(glGetUniformLocation(ShaderManager::programShadow, "lightDirection"), scene.SunDirection.x, scene.SunDirection.y, scene.SunDirection.z);

	glUniform1i(glGetUniformLocation(ShaderManager::programShadow, "shadowMap"), 0);
	glUniform1i(glGetUniformLocation(ShaderManager::programShadow, "diffuseMap"), 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glActiveTexture(GL_TEXTURE1);

	glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programShadow, "lightMatrix"), 1, GL_FALSE, &(scene.lightMatrix)[0][0]);

	for (int i = 0; i < scene.solidObjects.size(); i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programShadow, "MVP"), 1, GL_FALSE, &(MVP*scene.solidObjects[i]->getModelMatrix())[0][0]); 
		glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programShadow, "modelMatrix"), 1, GL_FALSE, &scene.solidObjects[i]->getModelMatrix()[0][0]);
		//box_node.draw();
		scene.solidObjects[i]->draw();
	}

	glUseProgram(ShaderManager::programShadowInstanced);
	glUniform1f(glGetUniformLocation(ShaderManager::programShadowInstanced, "heightOffset"), 0);
	renderInstanced(scene, MVP, true);

	glUseProgram(ShaderManager::programGlow);
	
	glActiveTexture(GL_TEXTURE0);

	for (int i = 0; i < scene.glowingObjects.size(); i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programGlow, "MVP"), 1, GL_FALSE, &(MVP*scene.glowingObjects[i]->getModelMatrix())[0][0]);
		glUniform3f(glGetUniformLocation(ShaderManager::programGlow, "lightColor"), 3.0, 3.0, 3.0);
		scene.glowingObjects[i]->draw();
	}
		
	renderTerrain(scene, MVP, true);

	if (scene.water)
	{
		glUseProgram(ShaderManager::programWater);
		glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programWater, "VP"), 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programWater, "V"), 1, GL_FALSE, &scene.camera.ViewMatrix()[0][0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, scene.water->m_reflectionTex);

		scene.water->draw();
	}


	/*glUseProgram(ShaderManager::programMirror); 
	
	glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programMirror, "MVP"), 1, GL_FALSE, &(MVP*scene.mirror.modelMatrix())[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programMirror, "modelMatrix"), 1, GL_FALSE, &scene.mirror.modelMatrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programMirror, "V"), 1, GL_FALSE, &MVP[0][0]);
	//glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programMirror, "V"), 1, GL_FALSE, &scene.camera.ViewMatrix()[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, scene.mirror.m_reflectionTex);

	glDisable(GL_CULL_FACE);
	scene.mirror.draw();
	glEnable(GL_CULL_FACE);*/


	/*uniform mat4 MVP;
	uniform mat4 modelMatrix;
	uniform sampler2D Texture;
	uniform vec3 lightDirection;*/

	glBindTexture(GL_TEXTURE_2D, 0);
}
