#ifndef __MODULEPROGRAM_H__
#define __MODULEPROGRAM_H__

#include "Module.h"
#include "assimp\cimport.h"
#include <map>

class ProgramManager
{
private:

	struct LessString {
		bool operator()(const aiString& left, const aiString& right) {
			return ::strcmp(left.data, right.data) < 0;
		}
	};

	typedef std::map<aiString, LessString> ProgramList;

	ProgramList programs;
	static std::auto_ptr<ProgramManager> instance;

public:
	ProgramManager();
	~ProgramManager();

	void Load(const char* name, const char* vertexShaderPath, const char* fragmentShaderPath);
	void Clear();

	int GetUniformLocation(const char* name, const char* uniform);
	void UseProgram(const char* name);
	void UnuseProgram();

	char* ReadShader(const char* name);

	static ProgramManager* GetInstance();
};

#endif