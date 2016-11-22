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

	glUniform3f(glGetUniformLocation(*program, "lightDirection"), scene.SunDirection.x, scene.SunDirection.y, scene.SunDirection.z);
	glUniformMatrix4fv(glGetUniformLocation(*program, "MVP"), 1, GL_FALSE, &matrix[0][0]);

	if (shadow)
	{
		glUniformMatrix4fv(glGetUniformLocation(*program, "lightMatrix"), 1, GL_FALSE, &scene.lightMatrix[0][0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
	}

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

void RenderEngine::renderSolids(Scene & scene, glm::mat4 & matrix, bool shadow)
{
	GLuint *program;

	shadow ?
		program = &ShaderManager::programShadow
		:
		program = &ShaderManager::program;


	glUseProgram(*program);
	glUniform3f(glGetUniformLocation(*program, "lightDirection"), scene.SunDirection.x, scene.SunDirection.y, scene.SunDirection.z);
	
	if (shadow)
	{
		glUniform1i(glGetUniformLocation(*program, "shadowMap"), 0);
		glUniform1i(glGetUniformLocation(*program, "diffuseMap"), 1);

		glUniformMatrix4fv(glGetUniformLocation(*program, "lightMatrix"), 1, GL_FALSE, &(scene.lightMatrix)[0][0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glActiveTexture(GL_TEXTURE1);
	}

	for (int i = 0; i < scene.solidObjects.size(); i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(*program, "VP"), 1, GL_FALSE, &matrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(*program, "modelMatrix"), 1, GL_FALSE, &scene.solidObjects[i]->getModelMatrix()[0][0]);
		scene.solidObjects[i]->draw();
	}

}

void RenderEngine::renderGlow(Scene & scene, glm::mat4 matrix)
{

	glUseProgram(ShaderManager::programGlow);
	glUniform3f(glGetUniformLocation(ShaderManager::programGlow, "lightColor"), 3.0, 3.0, 3.0);

	glActiveTexture(GL_TEXTURE0);

	for (int i = 0; i < scene.glowingObjects.size(); i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(ShaderManager::programGlow, "MVP"), 1, GL_FALSE, &(matrix* scene.glowingObjects[i]->getModelMatrix())[0][0]);
		scene.glowingObjects[i]->draw();
	}
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

void RenderEngine::renderWater(Scene & scene, glm::mat4 & matrix, bool shadow)
{
	if (scene.water)
	{
		GLuint *program = shadow ? &ShaderManager::programWaterShadow : &ShaderManager::programWater;

		glUseProgram(ShaderManager::programWaterShadow);
		glUniformMatrix4fv(glGetUniformLocation(*program, "VP"), 1, GL_FALSE, &matrix[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(*program, "V"), 1, GL_FALSE, &scene.camera.ViewMatrix()[0][0]);

		glUniform1i(glGetUniformLocation(*program, "reflection"), 0);
		glUniform1i(glGetUniformLocation(*program, "refraction"), 1);

		glUniform3f(glGetUniformLocation(*program, "CamPosition"), scene.camera.cameraGlobalPosition().x, scene.camera.cameraGlobalPosition().y, scene.camera.cameraGlobalPosition().z);
		glUniform3f(glGetUniformLocation(*program, "lightDirection"), scene.SunDirection.x, scene.SunDirection.y, scene.SunDirection.z);

		if (shadow)
		{
			glUniformMatrix4fv(glGetUniformLocation(*program, "lightMatrix"), 1, GL_FALSE, &(scene.lightMatrix)[0][0]);
			glUniform1i(glGetUniformLocation(*program, "shadowMap"), 3);

			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, depthMap);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, scene.water->m_reflectionTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, scene.water->m_refractionTex);
		

		scene.water->draw();


		glActiveTexture(GL_TEXTURE0);
	}
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
	if (scene.water)
	{
		// Water Reflection FBO
		glViewport(0, 0, 1280, 720);

		glm::mat4 MVP = scene.water->m_reflectionMatrix;

		glBindFramebuffer(GL_FRAMEBUFFER, scene.water->m_fbo[0]);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glClearColor(11 / 255.0, 176 / 255.0, 226 / 255.0, 1.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(ShaderManager::programTerrain);

		glUniform1f(glGetUniformLocation(ShaderManager::programTerrain, "heightOffset"), -scene.water->height());
		glUniform2f(glGetUniformLocation(ShaderManager::programTerrain, "heightTest"), 1.0f, scene.water->height());
		renderTerrain(scene, MVP, false);
		glUniform1f(glGetUniformLocation(ShaderManager::programTerrain, "heightOffset"), 0);

		glUseProgram(ShaderManager::programInstanced);
		glUniform1f(glGetUniformLocation(ShaderManager::programInstanced, "heightOffset"), -scene.water->height());
		glUniform2f(glGetUniformLocation(ShaderManager::programInstanced, "heightTest"), 1.0f, scene.water->height());
		renderInstanced(scene, MVP, false);
		glUniform1f(glGetUniformLocation(ShaderManager::programInstanced, "heightOffset"), 0);


		glUseProgram(ShaderManager::program);
		glUniform1f(glGetUniformLocation(ShaderManager::program, "heightOffset"), -scene.water->height());
		glUniform2f(glGetUniformLocation(ShaderManager::program, "heightTest"), 1.0f, scene.water->height());
		renderSolids(scene, MVP, false);
		glUniform1f(glGetUniformLocation(ShaderManager::program, "heightOffset"), 0);

		// Water Refraction FBO
		glViewport(0, 0, 1280, 720);

		MVP = scene.water->m_refractionMatrix;

		glBindFramebuffer(GL_FRAMEBUFFER, scene.water->m_fbo[1]);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glClearColor(11 / 255.0, 176 / 255.0, 226 / 255.0, 1.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(ShaderManager::programTerrain);

		glUniform2f(glGetUniformLocation(ShaderManager::programTerrain, "heightTest"), -1.0f, scene.water->height() + 0.1);
		renderTerrain(scene, MVP, false);
		glUniform2f(glGetUniformLocation(ShaderManager::programTerrain, "heightTest"), 1.0f, scene.water->height());


		glUseProgram(ShaderManager::programInstanced);
		glUniform2f(glGetUniformLocation(ShaderManager::programInstanced, "heightTest"), -1.0f, scene.water->height() + .1);
		renderInstanced(scene, MVP, false);
		glUniform2f(glGetUniformLocation(ShaderManager::programInstanced, "heightTest"), 1.0f, scene.water->height());

		glUseProgram(ShaderManager::program);
		glUniform2f(glGetUniformLocation(ShaderManager::program, "heightTest"), -1.0f, scene.water->height() + 0.1);
		renderSolids(scene, MVP, false);
		glUniform2f(glGetUniformLocation(ShaderManager::program, "heightTest"), 1.0f, scene.water->height());
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, WindowManager::width(), WindowManager::height());

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(11/ 255.0, 176/ 255.0, 226 / 255.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 VP = WindowManager::projectionMatrix() * scene.camera.ViewMatrix();
	//glm::mat4 VP = scene.water->m_refractionMatrix;

	renderSolids(scene, VP, false);
	renderInstanced(scene, VP, false);
	renderGlow(scene, VP);
	renderTerrain(scene, VP, false);
	renderWater(scene, VP, false);
}

void RenderEngine::drawShaded(Scene & scene)
{
	renderShadowMap(scene);

	glm::mat4 VP;
	
	if (scene.water)
	{
		// Water Reflection FBO
		glm::mat4 MVP = scene.water->m_reflectionMatrix;

		glViewport(0, 0, 1280, 720);

		glBindFramebuffer(GL_FRAMEBUFFER, scene.water->m_fbo[0]);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glClearColor(11 / 255.0, 176 / 255.0, 226 / 255.0, 1.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glUseProgram(ShaderManager::programTerrainShadow);
		glUniform1f(glGetUniformLocation(ShaderManager::programTerrainShadow, "heightOffset"), -scene.water->height());
		glUniform2f(glGetUniformLocation(ShaderManager::programTerrainShadow, "heightTest"), 1.0f, scene.water->height());
		renderTerrain(scene, MVP, true);
		glUniform1f(glGetUniformLocation(ShaderManager::programTerrainShadow, "heightOffset"), 0);
		
		glUseProgram(ShaderManager::programShadowInstanced);
		glUniform1f(glGetUniformLocation(ShaderManager::programShadowInstanced, "heightOffset"), -scene.water->height());
		glUniform2f(glGetUniformLocation(ShaderManager::programShadowInstanced, "heightTest"), 1.0f, scene.water->height());
		renderInstanced(scene, MVP, true);
		glUniform1f(glGetUniformLocation(ShaderManager::programShadowInstanced, "heightOffset"), 0);
		
		glUseProgram(ShaderManager::programShadow);
		glUniform1f(glGetUniformLocation(ShaderManager::programShadow, "heightOffset"), -scene.water->height());
		glUniform2f(glGetUniformLocation(ShaderManager::programShadow, "heightTest"), 1.0f, scene.water->height());
		renderSolids(scene, MVP, true);
		glUniform1f(glGetUniformLocation(ShaderManager::programShadow, "heightOffset"), 0);

		// Water Refraction FBO
		glViewport(0, 0, 1280, 720);

		MVP = scene.water->m_refractionMatrix;

		glBindFramebuffer(GL_FRAMEBUFFER, scene.water->m_fbo[1]);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glClearColor(11 / 255.0, 176 / 255.0, 226 / 255.0, 1.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(ShaderManager::programTerrainShadow);

		glUniform2f(glGetUniformLocation(ShaderManager::programTerrainShadow, "heightTest"), -1.0f, scene.water->height() + 0.1);
		renderTerrain(scene, MVP, true);
		glUniform2f(glGetUniformLocation(ShaderManager::programTerrainShadow, "heightTest"), 1.0f, scene.water->height());


		glUseProgram(ShaderManager::programShadowInstanced);
		glUniform2f(glGetUniformLocation(ShaderManager::programShadowInstanced, "heightTest"), -1.0f, scene.water->height() + .1);
		renderInstanced(scene, MVP, true);
		glUniform2f(glGetUniformLocation(ShaderManager::programShadowInstanced, "heightTest"), 1.0f, scene.water->height());

		glUseProgram(ShaderManager::programShadow);
		glUniform2f(glGetUniformLocation(ShaderManager::programShadow, "heightTest"), -1.0f, scene.water->height() + 0.1);
		renderSolids(scene, MVP, true);
		glUniform2f(glGetUniformLocation(ShaderManager::programShadow, "heightTest"), 1.0f, scene.water->height());
	}

	glViewport(0, 0, WindowManager::width(), WindowManager::height());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(11 / 255.0, 176 / 255.0, 226 / 255.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	VP = WindowManager::projectionMatrix() * scene.camera.ViewMatrix();

	// = = = = = = DRAW = = = = = =

	renderSolids(scene, VP, true);
	renderInstanced(scene, VP, true);
	renderGlow(scene, VP);
	renderTerrain(scene, VP, true);
	renderWater(scene, VP, true);

	glBindTexture(GL_TEXTURE_2D, 0);
}
