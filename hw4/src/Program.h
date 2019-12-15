#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>

class Program
{
public:
	
	Program();
	~Program();
	void SetShadersFileName(char *vFileName, char *sFileName);
	void CheckShaderCompileStatus(GLuint shader);
	void Init();
	std::string ReadShader(const char *name);
	void SendAttributeData(std::vector<float> &buffer, const char* name);
	void SendUniformData(int a, const char* name);
	void SendUniformData(float a, const char* name);
	void SendUniformData(glm::vec3 input, const char* name);
	void SendUniformData(glm::mat4 &mat, const char* name);
	void SendUniformData(glm::mat3 &mat, const char* name);
	void Bind();
	void Unbind();


private:
	GLint programID;
	char *vertexShaderFileName, *fragmentShaderFileName;
};

