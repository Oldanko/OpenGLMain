#include "RenderEngine.h"
#include "ShaderManager.h"


GLuint RenderEngine::shadowMapFBO;
GLuint RenderEngine::shadowMap;

GLuint RenderEngine::gFBO;
GLuint RenderEngine::gRBO;
GLuint RenderEngine::gColor;
GLuint RenderEngine::gNormPos;

GLuint RenderEngine::MatrixID = 0;
GLuint RenderEngine::ViewMatrixID = 1;
GLuint RenderEngine::ModelMatrixID = 2;
GLuint RenderEngine::LightDirectionID = 3;

GLuint RenderEngine::gColorID = 0;
GLuint RenderEngine::gNormPosID = 1;


void RenderEngine::renderSolids(Scene & scene, glm::mat4 & matrix)
{
	GLuint *program = &ShaderManager::program;

	glUseProgram(*program);

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &matrix[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &scene.camera.ViewMatrix()[0][0]);

	for (int i = 0; i < scene.solidObjects.size(); i++)
	{
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &scene.solidObjects[i]->getModelMatrix()[0][0]);
		glActiveTexture(GL_TEXTURE0);
		scene.solidObjects[i]->draw();
	}
}

void RenderEngine::renderInstanced(Scene & scene, glm::mat4 & matrix)
{

	GLuint *program = &ShaderManager::programInstanced;

	glUseProgram(*program);

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &matrix[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &scene.camera.ViewMatrix()[0][0]);

	for (int i = 0; i < scene.solidObjectsInstanced.size(); i++)
	{
		scene.solidObjectsInstanced[i]->draw();
	}

	glDisable(GL_CULL_FACE);
	scene.grass->draw();
	//for (int i = 0; i < scene.grass.size(); i++)
		//scene.grass[i]->draw();
	glEnable(GL_CULL_FACE);
}

void RenderEngine::renderTerrain(Scene &scene, glm::mat4 &matrix)
{

	GLuint *program = &ShaderManager::programTerrain;

	glUseProgram(*program);

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &matrix[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &scene.camera.ViewMatrix()[0][0]);

	glBindTexture(GL_TEXTURE_2D, gColor);
	scene.terrain.draw();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderEngine::renderWater(Scene & scene, glm::mat4 & matrix)
{

	GLuint *program = &ShaderManager::programWater;

	glUseProgram(*program);

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &matrix[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &scene.camera.ViewMatrix()[0][0]);

	glUniform1f(2, 1.4f);
	glUniform3f(3, 0.0f, 1.0f, 0.0f);
	scene.water->draw();


}

void RenderEngine::renderSolidsReflection(Scene & scene, glm::mat4 & matrix, bool reflection)
{

	GLuint *program = &ShaderManager::programReflection;
	
	glUseProgram(*program);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &matrix[0][0]);

	glm::vec3 surfNormal;
	glm::vec3 surfPosition;

	if (reflection)
	{
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &scene.water->m_reflectionMatrix[0][0]);
		surfNormal = glm::vec3(scene.water->m_reflectionMatrix * glm::vec4(0, -1, 0, 0));
		surfPosition = glm::vec3(scene.water->m_reflectionMatrix * glm::vec4(0, 1.4, 0, 1));
	}
	else
	{
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &scene.camera.ViewMatrix()[0][0]);
		surfNormal = glm::vec3(scene.camera.ViewMatrix() * glm::vec4(0, 1, 0, 0));
		surfPosition = glm::vec3(scene.camera.ViewMatrix() * glm::vec4(0, 1.4, 0, 1));
	}

	glUniform3fv(3, 1, &surfPosition[0]);
	glUniform3fv(4, 1, &surfNormal[0]);

	for (int i = 0; i < scene.solidObjects.size(); i++)
	{
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &scene.solidObjects[i]->getModelMatrix()[0][0]);
		scene.solidObjects[i]->draw();
	}

}

void RenderEngine::renderInstancedReflection(Scene & scene, glm::mat4 & matrix, bool reflection)
{

	GLuint *program = &ShaderManager::programInstancedReflection;

	glUseProgram(*program);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &matrix[0][0]);
	
	glm::vec3 surfNormal;
	glm::vec3 surfPosition;

	if (reflection)
	{
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &scene.water->m_reflectionMatrix[0][0]);
		surfNormal = glm::vec3(scene.water->m_reflectionMatrix * glm::vec4(0, -1, 0, 0));
		surfPosition = glm::vec3(scene.water->m_reflectionMatrix * glm::vec4(0, 1.4, 0, 1));
	}
	else
	{
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &scene.camera.ViewMatrix()[0][0]);
		surfNormal = glm::vec3(scene.camera.ViewMatrix() * glm::vec4(0, 1, 0, 0));
		surfPosition = glm::vec3(scene.camera.ViewMatrix() * glm::vec4(0, 1.4, 0, 1));
	}

	glUniform3fv(3, 1, &surfPosition[0]);
	glUniform3fv(4, 1, &surfNormal[0]);

	for (int i = 0; i < scene.solidObjectsInstanced.size(); i++)
	{
		scene.solidObjectsInstanced[i]->draw();
	}

	glDisable(GL_CULL_FACE);
	scene.grass->draw();
	//for (int i = 0; i < scene.grass.size(); i++)
		//scene.grass[i]->draw();
	glEnable(GL_CULL_FACE);
}

void RenderEngine::renderTerrainReflection(Scene &scene, glm::mat4 &matrix, bool reflection)
{

	GLuint *program = &ShaderManager::programTerrainReflection;
	
	glUseProgram(*program);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &matrix[0][0]);

	glm::vec3 surfNormal;
	glm::vec3 surfPosition;

	if (reflection)
	{
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &scene.water->m_reflectionMatrix[0][0]);
		surfNormal = glm::vec3(scene.water->m_reflectionMatrix * glm::vec4(0, -1, 0, 0));
		surfPosition = glm::vec3(scene.water->m_reflectionMatrix * glm::vec4(0, 1.4, 0, 1));
	}
	else
	{
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &scene.camera.ViewMatrix()[0][0]);
		surfNormal = glm::vec3(scene.camera.ViewMatrix() * glm::vec4(0, 1, 0, 0));
		surfPosition = glm::vec3(scene.camera.ViewMatrix() * glm::vec4(0, 1.4, 0, 1));
	}

	glUniform3fv(3, 1, &surfPosition[0]);
	glUniform3fv(4, 1, &surfNormal[0]);

	glBindTexture(GL_TEXTURE_2D, gColor);
	scene.terrain.draw();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderEngine::renderShadowMap(Scene & scene)
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(ShaderManager::programDepth);

	for (int i = 0; i < scene.solidObjects.size(); i++)
	{
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(scene.lightMatrix*scene.solidObjects[i]->getModelMatrix())[0][0]);
		scene.solidObjects[i]->draw();
	}

	glUseProgram(ShaderManager::programInstancedDepth);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &scene.lightMatrix[0][0]);

	for (int i = 0; i < scene.solidObjectsInstanced.size(); i++)
	{
		scene.solidObjectsInstanced[i]->draw();
	}
	glUseProgram(ShaderManager::programTerrainDepth);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &scene.lightMatrix[0][0]);
	scene.terrain.draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void RenderEngine::init()
{
	glGenFramebuffers(1, &shadowMapFBO);

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Texture::textures["shadowMap"] = new Texture(shadowMap);

	glGenFramebuffers(1, &gFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, gFBO);

	glGenTextures(1, &gColor);
	glBindTexture(GL_TEXTURE_2D, gColor);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, WindowManager::width(), WindowManager::height());
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gColor, 0);

	glGenTextures(1, &gNormPos);
	glBindTexture(GL_TEXTURE_2D, gNormPos);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32UI, WindowManager::width(), WindowManager::height());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormPos, 0);

	/*glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WindowManager::width(), WindowManager::height(), 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gPosition, 0);*/

	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	glGenRenderbuffers(1, &gRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, gRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WindowManager::width(), WindowManager::height());
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, gRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	std::cout << gColor << "\n";

	Texture::textures["gColor"] = new Texture(gColor);
	Texture::textures["gNormPos"] = new Texture(gNormPos);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glUseProgram(ShaderManager::programLighting);
	glUniform1i(glGetUniformLocation(ShaderManager::programLighting, "Color"), 0);
	glUniform1i(glGetUniformLocation(ShaderManager::programLighting, "NormPos"), 1);
	glUniform1i(glGetUniformLocation(ShaderManager::programLighting, "ShadowMap"), 2);


	glUseProgram(ShaderManager::programReflection);
	glUniform1i(glGetUniformLocation(ShaderManager::programReflection, "Texture"), 0);

	glUseProgram(ShaderManager::programInstancedReflection);
	glUniform1i(glGetUniformLocation(ShaderManager::programInstancedReflection, "Texture"), 0);

	glUseProgram(ShaderManager::programTerrainReflection);
	glUniform1i(glGetUniformLocation(ShaderManager::programTerrainReflection, "noiseTex"), 0);

	glUseProgram(0);
}

void RenderEngine::terminate()
{
	glDeleteTextures(1, &shadowMap);
	glDeleteFramebuffers(1, &shadowMapFBO);

	glDeleteTextures(1, &gColor);
	glDeleteTextures(1, &gNormPos);

	glDeleteFramebuffers(1, &gFBO);
	glDeleteRenderbuffers(1, &gRBO);
}

void RenderEngine::render(Scene & scene)
{
	renderShadowMap(scene);

	glm::mat4 VP;

	// /////////////////////////////////////

	VP = WindowManager::projectionMatrix() * scene.camera.ViewMatrix();

	glBindFramebuffer(GL_FRAMEBUFFER, gFBO);
	glViewport(0, 0, WindowManager::width(), WindowManager::height());

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderWater(scene, VP);
	//renderSolids(scene, VP);
	renderInstanced(scene, VP);
	renderTerrain(scene, VP);

	// /////////////////////////////////////
	/*
	auto deferredPathRefl= [&scene, &VP](bool isReflection) {
		glBindFramebuffer(GL_FRAMEBUFFER, scene.water->m_gFBO);
		glViewport(0, 0, WindowManager::width(), WindowManager::height());

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);


		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		renderTerrainReflection(scene, VP, isReflection);
		renderSolidsReflection(scene, VP, isReflection);
		renderInstancedReflection(scene, VP, isReflection);
	};
	auto lightingPathRefl = [&scene](GLuint fbo, glm::mat4 &matrix, GLuint isReflection) {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glViewport(0, 0, WindowManager::width(), WindowManager::height());

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glClearColor(11 / 255.0, 176 / 255.0, 226 / 255.0, 1.0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(ShaderManager::programLightingReflection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, scene.water->m_gTex[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, scene.water->m_gTex[1]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, scene.water->m_gTex[2]);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gMaterial);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, shadowMap);

		//glUniformMatrix4fv(1, 1, GL_FALSE, &(scene.lightMatrix * inverse(scene.water->m_reflectionMatrix))[0][0]);
		//glUniform3fv(LightDirectionID, 1, &glm::normalize(glm::vec3(scene.water->m_reflectionMatrix * glm::vec4(scene.SunDirection, 0.0)))[0]);

		glUniformMatrix4fv(1, 1, GL_FALSE, &(scene.lightMatrix * inverse(matrix))[0][0]);
		glUniform3fv(LightDirectionID, 1, &glm::normalize(glm::vec3(matrix * glm::vec4(scene.SunDirection, 0.0)))[0]);
		glUniform1ui(glGetUniformLocation(ShaderManager::programLightingReflection, "isReflection"), isReflection);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	};

	VP = WindowManager::projectionMatrix() * scene.water->m_reflectionMatrix;

	deferredPathRefl(true);
	lightingPathRefl(scene.water->m_fbo[0], scene.water->m_reflectionMatrix, 1);

	VP = WindowManager::projectionMatrix() * scene.camera.ViewMatrix();

	deferredPathRefl(false);
	lightingPathRefl(scene.water->m_fbo[1], scene.camera.ViewMatrix(), 0);
	*/
	// /////////////////////////////////////

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WindowManager::width(), WindowManager::height());

	glClearColor(11 / 255.0, 176 / 255.0, 226 / 255.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(ShaderManager::programLighting);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gColor);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormPos);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	glUniform3fv(LightDirectionID, 1, &glm::normalize(glm::vec3(scene.camera.ViewMatrix() * glm::vec4(scene.SunDirection, 0.0)))[0]);
	glUniformMatrix4fv(1, 1, GL_FALSE, &(scene.lightMatrix * inverse(scene.camera.ViewMatrix()))[0][0]);
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	renderSolids(scene, VP);

}