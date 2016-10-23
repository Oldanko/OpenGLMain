#include "ShaderManager.h"

#include <vector>
#include <fstream>
#include <string>


GLuint ShaderManager::program = 0;
GLuint ShaderManager::programInstanced = 0;
GLuint ShaderManager::programGlow = 0;

GLuint ShaderManager::programDepth = 0;
GLuint ShaderManager::programDepthInstanced = 0;
GLuint ShaderManager::programTerrainDepth = 0;

GLuint ShaderManager::program2D = 0;

GLuint ShaderManager::programShadow = 0; 
GLuint ShaderManager::programShadowInstanced = 0;

GLuint ShaderManager::programTerrain = 0;
GLuint ShaderManager::programTerrainShadow = 0;

GLuint ShaderManager::programWater = 0; 

void ShaderManager::init()
{
	program = LoadShaders("shader.vertex", "shader.fragment");
	programInstanced = LoadShaders("shaderInstanced.vertex", "shader.fragment");
	programGlow = LoadShaders("shader.vertex", "shaderGlow.fragment");

	programDepth = LoadShaders("shaderDepth.vertex", "shaderEmpty.fragment");
	programDepthInstanced = LoadShaders("shaderDepthInstanced.vertex", "shaderEmpty.fragment");
	programTerrainDepth = LoadShaders("shaderTerrainDepth.vertex", "shaderEmpty.fragment");

	program2D = LoadShaders("shader2D.vertex", "shader2D.fragment");

	programShadow = LoadShaders("shaderShadow.vertex", "shaderShadow.fragment");
	programShadowInstanced = LoadShaders("shaderShadowInstanced.vertex", "shaderShadow.fragment");

	programTerrain = LoadShaders("shaderTerrain.vertex", "shaderTerrain.fragment");
	programTerrainShadow = LoadShaders("shaderTerrainShadow.vertex", "shaderTerrainShadow.fragment");

	programWater = LoadShaders("shaderWater.vertex", "shaderWater.fragment");
}

void ShaderManager::terminate()
{
	glDeleteProgram(program);
	glDeleteProgram(programInstanced);
	glDeleteProgram(programGlow);
	glDeleteProgram(programDepth);
	glDeleteProgram(programDepthInstanced);

	glDeleteProgram(program2D);

	glDeleteProgram(programShadow);
	glDeleteProgram(programShadowInstanced);

	glDeleteProgram(programWater);
}




GLuint ShaderManager::LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);

	if (VertexShaderStream.is_open()) {

		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;

		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}
	// Read the Fragment Shader code from the file

	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);

	if (FragmentShaderStream.is_open()) {

		std::string Line = "";

		while (getline(FragmentShaderStream, Line))

			FragmentShaderCode += "\n" + Line;

		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();

	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);

	char const * FragmentSourcePointer = FragmentShaderCode.c_str();

	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;

}