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

void ProgramManager::Load(const std::string name, const std::string vertexShaderPath, const std::string fragmentShaderPath)
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
	//Check Vertex Shader
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &logLength);
	char* vertShaderError = new char[logLength];
	glGetShaderInfoLog(vertexShaderId, logLength, NULL, vertShaderError);
	DLOG("Check result is: %d", result);
	DLOG(vertShaderError);


	//Compile Fragment Shader
	DLOG("Compiling fragment shader.");
	glShaderSource(fragmentShaderId, 1, &fragmentShader, NULL);
	glCompileShader(fragmentShaderId);
	//Check Fragment Shader
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &logLength);
	char* fragShaderError = new char[logLength];
	glGetShaderInfoLog(fragmentShaderId, logLength, NULL, fragShaderError);
	DLOG("Check result is: %d",result);
	DLOG(fragShaderError);

	//Create Program
	DLOG("Linking program.");
	GLuint id = glCreateProgram();
	glAttachShader(id, vertexShaderId);
	glAttachShader(id, fragmentShaderId);
	glLinkProgram(id);
	//Check Program
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
	char* programError = new char[logLength];
	glGetProgramInfoLog(id, logLength, NULL, programError);
	DLOG("Check result is: %d", result);
	DLOG(programError);

	//Use this program for everything to test
	//glUseProgram(id);

	//Save Shader
	Program loadedProgram;
	loadedProgram.id = id;
	programs[name] = loadedProgram;

	GLint locUniform = glGetUniformLocation(id, "z");
	GLenum error = glGetError();

	//Releasing
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
	RELEASE(vertShaderError);
	RELEASE(fragShaderError);
	RELEASE(programError);
}

void ProgramManager::Clear()
{
	programs.clear();
	if (instance != nullptr) {
		RELEASE(instance);
		instance = nullptr;
	}
}

int ProgramManager::GetUniformLocation(const char * name, const char * uniform)
{
	return 0;
}

GLint ProgramManager::GetProgramId(const char * name)
{
	return programs[name].id;
}

void ProgramManager::UseProgram(const std::string name)
{
	ProgramList::iterator it = programs.find(name);
	if (it != programs.end()) {
		SetProgramsToFalse();
		glUseProgram(it->second.id);
		it->second.active = true;
	}
	else {
		DLOG("Program not found!");
	}
}

void ProgramManager::UnuseProgram()
{
	GLint data;
	glGetIntegerv(GL_CURRENT_PROGRAM,&data);
	if (data != 0) {
		glUseProgram(0);
		SetProgramsToFalse();
	}else {
		DLOG("No program active to unuse!");
	}
}

std::string ProgramManager::ReadShader(const std::string filePath) {
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

void ProgramManager::SetProgramsToFalse()
{
	for (ProgramList::iterator it = programs.begin(); it != programs.end(); it++) {
		it->second.active = false;
	}
}

ProgramManager * ProgramManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new ProgramManager();
	}
	return instance;
}
