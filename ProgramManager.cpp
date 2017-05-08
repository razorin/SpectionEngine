#include "ProgramManager.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

//http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/

ProgramManager* ProgramManager::instance = nullptr;

ProgramManager::ProgramManager()
{	
}


ProgramManager::~ProgramManager()
{
}

void ProgramManager::Load(const char * name, const char * vertexShaderPath, const char * fragmentShaderPath)
{
	//Create Shaders
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	//Read Shaders
	std::string vertexShaderStr = ReadShader(vertexShaderPath);
	std::string fragmentShaderStr = ReadShader(fragmentShaderPath);
	const char *vertexShader = vertexShaderStr.c_str();
	const char *fragmentShader = fragmentShaderStr.c_str();
	
	//Declare variables to get log info
	GLint result = GL_FALSE;
	int logLength;

	//Compile Vertex Shader
	DLOG("Compiling vertex shader.");
	glShaderSource(vertexShaderId, 1, &vertexShader, NULL);
	glCompileShader(vertexShaderId);
	// Check vertex shader
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &logLength);
	char* vertShaderError = new char[logLength];
	//char* error_message = new char[logLength];
	char error_message[1024];
	int real_size = 0;
	glGetShaderInfoLog(vertexShaderId, logLength, NULL, vertShaderError);
	DLOG(vertShaderError);


	//Compile Vertex Shader
	DLOG("Compiling fragment shader.");
	glShaderSource(fragmentShaderId, 1, &fragmentShader, NULL);
	glCompileShader(vertexShaderId);

	//glShaderSource(vertexShaderId,1,&vertexShader,NULL);
	
	glCompileShader(fragmentShaderId);
	//Create Program
	GLuint id = glCreateProgram();
	glAttachShader(id, vertexShaderId);
	glAttachShader(id, fragmentShaderId);
	glLinkProgram(id);
	

}

void ProgramManager::Clear()
{
}

int ProgramManager::GetUniformLocation(const char * name, const char * uniform)
{
	return 0;
}

void ProgramManager::UseProgram(const char * name)
{
	//glUseProgram(id);
}

void ProgramManager::UnuseProgram()
{
}

std::string ProgramManager::ReadShader(const char *filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);
	
	
	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}
	
	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	
	fileStream.close();
	return content;
}

ProgramManager * ProgramManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new ProgramManager();
	}
	return instance;
}
