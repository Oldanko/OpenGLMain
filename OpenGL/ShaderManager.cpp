#include "ShaderManager.h"

#include <vector>
#include <fstream>
#include <string>


GLuint ShaderManager::program = 0;
GLuint ShaderManager::programInstanced = 0;
GLuint ShaderManager::programTerrain = 0;
GLuint ShaderManager::programWater = 0;

GLuint ShaderManager::program2D = 0;

GLuint ShaderManager::programLighting = 0;

GLuint ShaderManager::programDepth = 0;
GLuint ShaderManager::programInstancedDepth = 0;
GLuint ShaderManager::programTerrainDepth = 0;


void ShaderManager::init()
{
	program = LoadShaders("resources/shaders/shader.vertex", "resources/shaders/shader.fragment");
	programInstanced = LoadShaders("resources/shaders/shaderInstanced.vertex", "resources/shaders/shader.fragment");
	programTerrain = LoadShaders("resources/shaders/shaderTerrain.vertex", "resources/shaders/shaderTerrain.fragment");
	programWater = LoadShaders("resources/shaders/shaderWater.vertex", "resources/shaders/shaderWater.fragment");
	
	program2D = LoadShaders("resources/shaders/shader2D.vertex", "resources/shaders/shader2D.fragment");

	programLighting = LoadShaders("resources/shaders/shaderLighting.vertex", "resources/shaders/shaderLighting.fragment");

	programDepth = LoadShaders("resources/shaders/shaderDepth.vertex", "resources/shaders/shaderEmpty.fragment");
	programInstancedDepth = LoadShaders("resources/shaders/shaderDepthInstanced.vertex", "resources/shaders/shaderEmpty.fragment");
	programTerrainDepth = LoadShaders("resources/shaders/shaderTerrainDepth.vertex", "resources/shaders/shaderEmpty.fragment");

}

void ShaderManager::terminate()
{
	glDeleteProgram(program);
	glDeleteProgram(programInstanced);
	glDeleteProgram(programTerrain);

	glDeleteProgram(programDepth);
	glDeleteProgram(programInstancedDepth);
	glDeleteProgram(programTerrainDepth);

	glDeleteProgram(program2D);

	glDeleteProgram(programLighting);
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